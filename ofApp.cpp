#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetColor(255);
	ofSetLineWidth(3);
	ofEnableDepthTest();

	this->cap.open("D:\\MP4\\Pexels Videos 2441067.mp4");
	this->cap_size = cv::Size(128, 72);

	this->image.allocate(this->cap_size.width, this->cap_size.height, OF_IMAGE_COLOR);
	this->frame = cv::Mat(cv::Size(this->image.getWidth(), this->image.getHeight()), CV_MAKETYPE(CV_8UC3, this->image.getPixels().getNumChannels()), this->image.getPixels().getData(), 0);

	this->number_of_frames = this->cap.get(cv::CAP_PROP_FRAME_COUNT);
	for (int i = 0; i < this->number_of_frames; i++) {

		cv::Mat src, tmp;
		this->cap >> src;
		if (src.empty()) {

			continue;
		}

		cv::resize(src, tmp, this->cap_size);
		cv::cvtColor(tmp, tmp, cv::COLOR_BGR2RGB);

		this->frame_list.push_back(tmp);
	}

	this->mesh.addVertex(glm::vec3(this->cap_size.width * -0.5, this->cap_size.height * -0.5, 0));
	this->mesh.addVertex(glm::vec3(this->cap_size.width * 0.5,  this->cap_size.height * -0.5, 0));
	this->mesh.addVertex(glm::vec3(this->cap_size.width * 0.5,  this->cap_size.height * 0.5, 0));
	this->mesh.addVertex(glm::vec3(this->cap_size.width * -0.5, this->cap_size.height * 0.5, 0));
	
	this->mesh.addTexCoord(glm::vec3(0, 0, 0));
	this->mesh.addTexCoord(glm::vec3(this->cap_size.width, 0, 0));
	this->mesh.addTexCoord(glm::vec3(this->cap_size.width, this->cap_size.height, 0));
	this->mesh.addTexCoord(glm::vec3(0, this->cap_size.height, 0));

	this->mesh.addIndex(0); mesh.addIndex(1); mesh.addIndex(2);
	this->mesh.addIndex(0); mesh.addIndex(2); mesh.addIndex(3);

}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(180);

	for (int x = this->cap_size.width * -5; x <= this->cap_size.width * 5; x += this->cap_size.width) {

		for (int y = this->cap_size.height * -5; y <= this->cap_size.height * 5; y += this->cap_size.height) {

			auto location = glm::vec3(x, y, 0);
			int n = int(ofMap(ofNoise(x * 0.5625 * 0.005, y * 0.005, ofGetFrameNum() * 0.0005), 0, 1, 0, this->number_of_frames * 3)) % this->number_of_frames;
			this->frame_list[n].copyTo(this->frame);
			this->image.update();

			ofPushMatrix();
			ofTranslate(location);

			ofSetColor(255);
			ofFill();
			this->image.bind();
			mesh.draw();
			this->image.unbind();

			ofSetColor(39);
			ofNoFill();
			ofDrawRectangle(this->cap_size.width * -0.5, this->cap_size.height * -0.5, this->cap_size.width, this->cap_size.height);

			ofPopMatrix();
		}
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}