#include <glog/logging.h>

int main(int argc, char* argv[]) {
	//init glog
	google::InitGoogleLogging(argv[0]);
	google::SetStderrLogging(google::GLOG_INFO);
	FLAGS_colorlogtostderr = true;

	LOG(INFO) << "glog info";
	LOG(WARNING) << "glog warning";
	LOG(ERROR) << "glog error";
	return 0;
}
