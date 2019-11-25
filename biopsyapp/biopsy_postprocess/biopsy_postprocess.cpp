/*******
*
* Copyright(c)<2018>, <Huawei Technologies Co.,Ltd>
*
* @version 1.0
*
* @date 2018-5-19
*/
#include "biopsy_postprocess.h"
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstring>
#include <string>
#include <regex>
#include <cmath>

#include "hiaiengine/log.h"
#include "hiaiengine/data_type_reg.h"
#include "ascenddk/ascend_ezdvpp/dvpp_process.h"

using hiai::Engine;
using namespace std::__cxx11;
using namespace ascend::presenter;

// register custom data type
HIAI_REGISTER_DATA_TYPE("FaceRecognitionInfo", FaceRecognitionInfo);
HIAI_REGISTER_DATA_TYPE("FaceRectangle", FaceRectangle);
HIAI_REGISTER_DATA_TYPE("FaceImage", FaceImage);

// constants
namespace {
//// parameters for drawing box and label begin////
// face box color
const uint8_t kFaceBoxColorR = 255;
const uint8_t kFaceBoxColorG = 190;
const uint8_t kFaceBoxColorB = 0;

// face box border width
const int kFaceBoxBorderWidth = 2;

// face label color
const uint8_t kFaceLabelColorR = 255;
const uint8_t kFaceLabelColorG = 255;
const uint8_t kFaceLabelColorB = 0;

// face label font
const double kFaceLabelFontSize = 0.7;
const int kFaceLabelFontWidth = 2;

// face label text prefix
const std::string kFaceLabelTextPrefix = "Face:";
const std::string kFaceLabelTextSuffix = "%";
//// parameters for drawing box and label end////

// port number range
const int32_t kPortMinNumber = 0;
const int32_t kPortMaxNumber = 65535;

// confidence range
const float kConfidenceMin = 0.0;
const float kConfidenceMax = 1.0;

// face detection function return value
const int32_t kFdFunSuccess = 0;
const int32_t kFdFunFailed = -1;

// need to deal results when index is 2
const int32_t kDealResultIndex = 2;

// each results size
const int32_t kEachResultSize = 7;

// attribute index
const int32_t kAttributeIndex = 1;

// score index
const int32_t kScoreIndex = 2;

// anchor_lt.x index
const int32_t kAnchorLeftTopAxisIndexX = 3;

// anchor_lt.y index
const int32_t kAnchorLeftTopAxisIndexY = 4;

// anchor_rb.x index
const int32_t kAnchorRightBottomAxisIndexX = 5;

// anchor_rb.y index
const int32_t kAnchorRightBottomAxisIndexY = 6;

// face attribute
const float kAttributeFaceLabelValue = 1.0;
const float kAttributeFaceDeviation = 0.00001;

// percent
const int32_t kScorePercent = 100;

// IP regular expression
const std::string kIpRegularExpression =
    "^((25[0-5]|2[0-4]\\d|[1]{1}\\d{1}\\d{1}|[1-9]{1}\\d{1}|\\d{1})($|(?!\\.$)\\.)){4}$";

// channel name regular expression
const std::string kChannelNameRegularExpression = "[a-zA-Z0-9/]+";
}

biopsy_postprocess::biopsy_postprocess() {
  fd_post_process_config_ = nullptr;
  presenter_channel_ = nullptr;
}

/**
* @ingroup hiaiengine
* @brief HIAI_DEFINE_PROCESS : implementaion of the engine
* @[in]: engine name and the number of input
*/
HIAI_StatusT biopsy_postprocess::Init(
    const hiai::AIConfig &config,
    const std::vector<hiai::AIModelDescription> &model_desc) {
    // get configurations
    if (fd_post_process_config_ == nullptr) {
      fd_post_process_config_ = std::make_shared<FaceDetectionPostConfig>();
    }
    // get parameters from graph.config
    for (int index = 0; index < config.items_size(); index++) {
      const ::hiai::AIConfigItem& item = config.items(index);
      const std::string& name = item.name();
      const std::string& value = item.value();
      std::stringstream ss;
      ss << value;
      if (name == "Confidence") {
        ss >> (*fd_post_process_config_).confidence;
        // validate confidence
        if (IsInvalidConfidence(fd_post_process_config_->confidence)) {
          HIAI_ENGINE_LOG(HIAI_GRAPH_INVALID_VALUE,
                          "Confidence=%s which configured is invalid.",
                          value.c_str());
          return HIAI_ERROR;
        }
      } else if (name == "PresenterIp") {
        // validate presenter server IP
        if (IsInValidIp(value)) {
          HIAI_ENGINE_LOG(HIAI_GRAPH_INVALID_VALUE,
                          "PresenterIp=%s which configured is invalid.",
                          value.c_str());
          return HIAI_ERROR;
        }
        ss >> (*fd_post_process_config_).presenter_ip;
      } else if (name == "PresenterPort") {
        ss >> (*fd_post_process_config_).presenter_port;
        // validate presenter server port
        if (IsInValidPort(fd_post_process_config_->presenter_port)) {
          HIAI_ENGINE_LOG(HIAI_GRAPH_INVALID_VALUE,
                          "PresenterPort=%s which configured is invalid.",
                          value.c_str());
          return HIAI_ERROR;
        }
      } else if (name == "ChannelName") {
        // validate channel name
        if (IsInValidChannelName(value)) {
          HIAI_ENGINE_LOG(HIAI_GRAPH_INVALID_VALUE,
                          "ChannelName=%s which configured is invalid.",
                          value.c_str());
          return HIAI_ERROR;
        }
        ss >> (*fd_post_process_config_).channel_name;
      }
      // else : nothing need to do
    }
    // call presenter agent, create connection to presenter server
    uint16_t u_port = static_cast<uint16_t>(fd_post_process_config_
        ->presenter_port);
    OpenChannelParam channel_param = { fd_post_process_config_->presenter_ip,
        u_port, fd_post_process_config_->channel_name, ContentType::kVideo };
    Channel *chan = nullptr;
    PresenterErrorCode err_code = OpenChannel(chan, channel_param);
    // open channel failed
    if (err_code != PresenterErrorCode::kNone) {
      HIAI_ENGINE_LOG(HIAI_GRAPH_INIT_FAILED,
                      "Open presenter channel failed, error code=%d", err_code);
      return HIAI_ERROR;
    }

    presenter_channel_.reset(chan);
    HIAI_ENGINE_LOG(HIAI_DEBUG_INFO, "End initialize!");
    return HIAI_OK;
}

bool biopsy_postprocess::IsInValidIp(const std::string &ip) {
  regex re(kIpRegularExpression);
  smatch sm;
  return !regex_match(ip, sm, re);
}

bool biopsy_postprocess::IsInValidPort(int32_t port) {
  return (port <= kPortMinNumber) || (port > kPortMaxNumber);
}

bool biopsy_postprocess::IsInValidChannelName(
    const std::string &channel_name) {
  regex re(kChannelNameRegularExpression);
  smatch sm;
  return !regex_match(channel_name, sm, re);
}

bool biopsy_postprocess::IsInvalidConfidence(float confidence) {
  return (confidence <= kConfidenceMin) || (confidence > kConfidenceMax);
}

bool biopsy_postprocess::IsInvalidResults(float attr, float score,
                                                const Point &point_lt,
                                                const Point &point_rb) {
  // attribute is not face (background)
  if (std::abs(attr - kAttributeFaceLabelValue) > kAttributeFaceDeviation) {
    return true;
  }

  // confidence check
  if ((score < fd_post_process_config_->confidence)
      || IsInvalidConfidence(score)) {
    return true;
  }

  // rectangle position is a point or not: lt == rb
  if ((point_lt.x == point_rb.x) && (point_lt.y == point_rb.y)) {
    return true;
  }
  return false;
}

int32_t biopsy_postprocess::SendImage(uint32_t height, uint32_t width,
                                            uint32_t size, u_int8_t *data, std::vector<DetectionResult>& detection_results) {
  int32_t status = kFdFunSuccess;
  // parameter
  ImageFrame image_frame_para;
  image_frame_para.format = ImageFormat::kJpeg;
  image_frame_para.width = width;
  image_frame_para.height = height;
  image_frame_para.size = size;
  image_frame_para.data = data;
  image_frame_para.detection_results = detection_results;

  PresenterErrorCode p_ret = PresentImage(presenter_channel_.get(),
                                            image_frame_para);
  // send to presenter failed
  if (p_ret != PresenterErrorCode::kNone) {
    HIAI_ENGINE_LOG(HIAI_ENGINE_RUN_ARGS_NOT_RIGHT,
                      "Send JPEG image to presenter failed, error code=%d",
                      p_ret);
    status = kFdFunFailed;
  }

  return status;
}

HIAI_StatusT biopsy_postprocess::HandleOriginalImage(
    const std::shared_ptr<FaceRecognitionInfo> &inference_res) {
  HIAI_StatusT status = HIAI_OK;
  // std::vector<NewImageParaT> img_vec = inference_res->imgs;
  // // dealing every original image
  // for (uint32_t ind = 0; ind < inference_res->b_info.batch_size; ind++) {
  //   uint32_t width = img_vec[ind].img.width;
  //   uint32_t height = img_vec[ind].img.height;
  //   uint32_t size = img_vec[ind].img.size;

  //   // call SendImage
  //   // 1. call DVPP to change YUV420SP image to JPEG
  //   // 2. send image to presenter
  //   vector<DetectionResult> detection_results;
  //   int32_t ret = SendImage(height, width, size, img_vec[ind].img.data.get(), detection_results);
  //   if (ret == kFdFunFailed) {
  //     status = HIAI_ERROR;
  //     continue;
  //   }
  // }
  return status;
}

bool biopsy_postprocess::IsSupportFormat(hiai::IMAGEFORMAT format) {
  return format == hiai::YUV420SP;
}

HIAI_StatusT biopsy_postprocess::ConvertImage(hiai::ImageData<u_int8_t>& org_img) {
  hiai::IMAGEFORMAT format = org_img.format;
  if (!IsSupportFormat(format)){
    HIAI_ENGINE_LOG(HIAI_ENGINE_RUN_ARGS_NOT_RIGHT,
                    "Format %d is not supported!", format);
    return HIAI_ERROR;
  }

  uint32_t width = org_img.width;
  uint32_t height = org_img.height;
  uint32_t img_size = org_img.size;

  // parameter
  ascend::utils::DvppToJpgPara dvpp_to_jpeg_para;
  dvpp_to_jpeg_para.format = JPGENC_FORMAT_NV12;
  dvpp_to_jpeg_para.level = 100;//控制质量
  dvpp_to_jpeg_para.resolution.height = height;
  dvpp_to_jpeg_para.resolution.width = width;
  ascend::utils::DvppProcess dvpp_to_jpeg(dvpp_to_jpeg_para);
  

  // call DVPP
  ascend::utils::DvppOutput dvpp_output;
  int32_t ret = dvpp_to_jpeg.DvppOperationProc(reinterpret_cast<char*>(org_img.data.get()),
                                                img_size, &dvpp_output);

  // failed, no need to send to presenter
  if (ret != 0) {
    HIAI_ENGINE_LOG(HIAI_ENGINE_RUN_ARGS_NOT_RIGHT,
                    "Failed to convert YUV420SP to JPEG, skip it.");
    return HIAI_ERROR;
  }

  // reset the data in img_vec
  org_img.data.reset(dvpp_output.buffer, default_delete<uint8_t[]>());
  org_img.size = dvpp_output.size;

  return HIAI_OK;
}

HIAI_StatusT biopsy_postprocess::HandleResults(
    const std::shared_ptr<FaceRecognitionInfo> &inference_res) {
    HIAI_StatusT status = HIAI_OK;
    std::vector<FaceImage> face_img_vec = inference_res->face_imgs;
    ConvertImage(inference_res->org_img);//转换为jpeg格式
    /*uint32_t width = inference_res->org_img.width;
    uint32_t height = inference_res->org_img.height;
    uint32_t img_size = inference_res->org_img.size;

    std::vector<DetectionResult> detection_results;//发送的结果
    DetectionResult oneResult;//存储一张面孔的左上、右下坐标
    for(int myind = 0; myind < inference_res->face_imgs.size(); myind++)
    {
      Point point_lt, point_rb;
      
      point_lt.x = inference_res->face_imgs[myind].rectangle.lt.x;
      point_lt.y= inference_res->face_imgs[myind].rectangle.lt.y;
      point_rb.x= inference_res->face_imgs[myind].rectangle.rb.x;
      point_rb.y= inference_res->face_imgs[myind].rectangle.rb.y;
      oneResult.lt = point_lt;
      oneResult.rb = point_rb;
      oneResult.result_text.append("pitch:");
      oneResult.result_text.append(to_string(inference_res->face_imgs[myind].infe_res.head_pose[0]));
      oneResult.result_text.append(",yaw:");
      oneResult.result_text.append(to_string(inference_res->face_imgs[myind].infe_res.head_pose[1]));
      oneResult.result_text.append(",roll:");
      oneResult.result_text.append(to_string(inference_res->face_imgs[myind].infe_res.head_pose[2]));

      detection_results.emplace_back(oneResult);
    }
	
	
    int32_t ret;
    ret = SendImage(height, width, img_size, inference_res->org_img.data.get(), detection_results);	
	*/
    // check send result
    ascend::presenter::proto::PresentImageRequest data;
    data.set_format(ascend::presenter::proto::ImageFormat::kImageFormatJpeg); 
    data.set_width(1280);
    data.set_height(720);
    unique_ptr<google::protobuf::Message> resp;
    data.set_data(string((char *)inference_res->org_img.data.get(),
		inference_res->org_img.size));
    if(face_img_vec.size() != 0){
		ascend::presenter::proto::Rectangle_Attr *lxre=nullptr;
		ascend::presenter::proto::Coordinate *po=nullptr;
		lxre = data.add_rectangle_list();
		lxre->mutable_left_top()->set_x(face_img_vec[0].rectangle.lt.x);
		lxre->mutable_left_top()->set_y(face_img_vec[0].rectangle.lt.y);
		lxre->mutable_right_bottom()->set_x(face_img_vec[0].rectangle.rb.x);
		lxre->mutable_right_bottom()->set_y(face_img_vec[0].rectangle.rb.y);   
		string s="";
		s = s +  "pitch:" + to_string(inference_res->face_imgs[0].infe_res.head_pose[0]);
		s = s + ",yaw:" + to_string(inference_res->face_imgs[0].infe_res.head_pose[1]);
		s = s + ",roll:" + to_string(inference_res->face_imgs[0].infe_res.head_pose[2]);
		lxre->set_label_text(s);
		for(int i=0;i<68;i++){
			po = data.add_point_list();
			if (face_img_vec[0].infe_res.face_points[i].x<0)
				face_img_vec[0].infe_res.face_points[i].x = 0;
			if (face_img_vec[0].infe_res.face_points[i].y<0)
				face_img_vec[0].infe_res.face_points[i].y = 0;
			po->set_x(face_img_vec[0].infe_res.face_points[i].x);
			po->set_y(face_img_vec[0].infe_res.face_points[i].y);
    	}
    }    
    PresenterErrorCode error_code = presenter_channel_->SendMessage(data, resp);
    //printf("st:%d",error_code);
	//if (ret == kFdFunFailed) {
    // status = HIAI_ERROR;
    //}
    

  // std::vector<NewImageParaT> img_vec = inference_res->imgs;
  // std::vector<OutputT> output_data_vec = inference_res->output_datas;

  // // dealing every image
  // for (uint32_t ind = 0; ind < inference_res->b_info.batch_size; ind++) {
  //   // result
  //   int32_t out_index = ind * kDealResultIndex;
  //   OutputT out = output_data_vec[out_index];
  //   std::shared_ptr<hiai::AISimpleTensor> result_tensor = std::make_shared<
  //       hiai::AISimpleTensor>();
  //   result_tensor->SetBuffer(out.data.get(), out.size);
  //   int32_t size = result_tensor->GetSize() / sizeof(float);
  //   float result[size];
  //   errno_t mem_ret = memcpy_s(result, sizeof(result),
  //                              result_tensor->GetBuffer(),
  //                              result_tensor->GetSize());

  //   //lzz
  //   // int myi = 0;
  //   // while(myi < size)
  //   // {
  //   //     printf("lzz:%f     ", result[myi]);
  //   //     HIAI_ENGINE_LOG(HIAI_ENGINE_RUN_ARGS_NOT_RIGHT,
  //   //                   "lzz:%f     ", result[myi]);
  //   //     myi++;
  //   // }

  //   // memory copy failed, skip this image
  //   if (mem_ret != EOK) {
  //     HIAI_ENGINE_LOG(HIAI_ENGINE_RUN_ARGS_NOT_RIGHT,
  //                     "handle results: memcpy_s() error=%d", mem_ret);
  //     continue;
  //   }

  //   uint32_t width = img_vec[ind].img.width;
  //   uint32_t height = img_vec[ind].img.height;
  //   uint32_t img_size = img_vec[ind].img.size;

  //   // every inference result needs 8 float
  //   // loop the result for every inference result
  //   std::vector<DetectionResult> detection_results;
  //   float *ptr = result;
  //   for (int32_t k = 0; k < size - kEachResultSize; k += kEachResultSize) {
  //     ptr = result + k;
  //     // attribute
  //     float attr = ptr[kAttributeIndex];
  //     // confidence
  //     float score = ptr[kScoreIndex];

  //     //Detection result
  //     DetectionResult one_result;
  //     // left top
  //     Point point_lt, point_rb;
  //     point_lt.x = ptr[kAnchorLeftTopAxisIndexX] * width;
  //     point_lt.y = ptr[kAnchorLeftTopAxisIndexY] * height;
  //     // right bottom
  //     point_rb.x = ptr[kAnchorRightBottomAxisIndexX] * width;
  //     point_rb.y = ptr[kAnchorRightBottomAxisIndexY] * height;

  //     one_result.lt = point_lt;
  //     one_result.rb = point_rb;

  //     // check results is valid
  //     if (IsInvalidResults(attr, score, point_lt, point_rb)) {
  //       continue;
  //     }
  //     HIAI_ENGINE_LOG(HIAI_DEBUG_INFO,
  //                     "score=%f, lt.x=%d, lt.y=%d, rb.x=%d, rb.y=%d", score,
  //                     point_lt.x, point_lt.y, point_rb.x, point_rb.y);
  //     int32_t score_percent =  score * kScorePercent;
  //     one_result.result_text.append(kFaceLabelTextPrefix);
  //     one_result.result_text.append(to_string(score_percent));
  //     one_result.result_text.append(kFaceLabelTextSuffix);

  //     // push back
  //     detection_results.emplace_back(one_result);
  //   }

  //   int32_t ret;
  //   ret = SendImage(height, width, img_size, img_vec[ind].img.data.get(), detection_results);

  //   // check send result
  //   if (ret == kFdFunFailed) {
  //     status = HIAI_ERROR;
  //   }
  // }
  return status;
}

HIAI_IMPL_ENGINE_PROCESS("biopsy_postprocess", biopsy_postprocess,INPUT_SIZE)
{
    if (arg0 == nullptr) {
    HIAI_ENGINE_LOG(HIAI_ENGINE_RUN_ARGS_NOT_RIGHT,
                    "Failed to process invalid message.");
    return HIAI_ERROR;
  }

  // check original image is empty or not
  std::shared_ptr<FaceRecognitionInfo> inference_res = std::static_pointer_cast<
      FaceRecognitionInfo>(arg0);
  // if (inference_res->imgs.empty()) {
  //   HIAI_ENGINE_LOG(
  //       HIAI_ENGINE_RUN_ARGS_NOT_RIGHT,
  //       "Failed to process invalid message, original image is null.");
  //   return HIAI_ERROR;
  // }

  // // inference failed, dealing original images
  // if (!inference_res->status) {
  //   HIAI_ENGINE_LOG(HIAI_OK, inference_res->msg.c_str());
  //   HIAI_ENGINE_LOG(HIAI_OK, "will handle original image.");
  //   return HandleOriginalImage(inference_res);
  // }

  // // inference success, dealing inference results
  return HandleResults(inference_res);
  //return HIAI_OK;
}
