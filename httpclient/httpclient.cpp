#include <httplib.h>
#include <iostream>
#include <stdio.h>
#define CA_CERT_FILE "./ca-bundle.crt"

#include <list>

using namespace std;
#pragma pack(1)
//设备遥测数据
typedef struct __tag_JsonBodyTelemetryData{
    char dev_id[128];   //设备ID
    char dev_num[32];   //设备号
    float d_x;
    float d_y;
    float veh_heading;
    int eng_on;
    int batt_lvl;
    int alm_lvl;
    int64_t gps_time;
}JSON_BODY_TELEMETRY_DATA_S;

//Json Body类型
typedef enum __tag_JsonBodyType{
    TELEMETRY_DATA,
    JSON_BODY_TYPE_MAX
} JSON_BODY_TYPE_E;

typedef struct __tag_JsonBodyBuffer {
    uint32_t id;
    JSON_BODY_TYPE_E body_type;
    char data[1024];
}JSON_BODY_BUFFER_S;

#pragma pack()

static const int S_JSON_BODY_BUFFER_MAX = 100;
string m_hostUrl;   //服务器地址
string m_token;     //设备令牌
uint32_t m_bodyId;
list<JSON_BODY_BUFFER_S*> m_jsonBodys;

void AddJsonBody(JSON_BODY_TYPE_E nJsonBodyType, void *pData, int nLen)
{
    if(nJsonBodyType >= JSON_BODY_TYPE_MAX || pData == NULL)
    {
        printf("Josn body type(%d) not support or input data is error!\n", nJsonBodyType);
        return;
    }

    if(nJsonBodyType == TELEMETRY_DATA && nLen != sizeof (JSON_BODY_TELEMETRY_DATA_S))
    {
        printf("Json body input data is error!\n");
        return;
    }

    JSON_BODY_BUFFER_S *pBody = (JSON_BODY_BUFFER_S*)malloc(sizeof (JSON_BODY_BUFFER_S));
    if(pBody == NULL)
    {
        printf("Malloc Json body memory failed!\n");
        return;
    }
    if(m_jsonBodys.size() >= S_JSON_BODY_BUFFER_MAX)
    {
        JSON_BODY_BUFFER_S *pDel = m_jsonBodys.front();
        m_jsonBodys.pop_front();
        free(pDel);
        pDel = NULL;
    }
    pBody->id = m_bodyId++;
    pBody->body_type = nJsonBodyType;
    memcpy(pBody->data, pData, nLen);
    m_jsonBodys.push_back(pBody);
}

int PostJsonBody(const char *pJsonBody)
{
    string url;
    string token;
    {
        url = m_hostUrl;
        token = m_token;
    }
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
    //https://demo.thingsboard.io/api/v1/6osoIZrRH5ShhmwRK6Aj/telemetry
    httplib::SSLClient cli("104.196.24.70", 443); //服务器地址需要去掉https:// ，否则无法连接到服务器，可先尝试是否可以ping通
  // httplib::SSLClient cli("localhost", 8080);
  // httplib::SSLClient cli("google.com");
  //httplib::SSLClient cli("www.baidu.com");
  cli.set_ca_cert_path(CA_CERT_FILE);
  cli.enable_server_certificate_verification(false); //如果认证失败可设置为false
#else
  httplib::Client cli("104.196.24.70", 80);
#endif

  char postPath[256];
  snprintf(postPath, sizeof (postPath), "/api/v1/%s/telemetry", token.c_str());
  auto res = cli.Post(postPath,  //提交路径
                    pJsonBody,
                    "application/json");

  if (res) {
    if (res->status == 200) {
      std::cout << res->body << std::endl;
      return 0;
    }
    return -1;
  } else {
    std::cout << "post failed" << std::endl;
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
    auto result = cli.get_openssl_verify_result();
    if (result) {
      std::cout << "verify error: " << X509_verify_cert_error_string(result) << std::endl;
    }
#endif
    auto err = res.error();
    std::cout << err << std::endl;
  }
  return -1;
}

int main(void)
{
    m_hostUrl = "104.196.24.70";
    m_token = "6osoIZrRH5ShhmwRK6Aj";
#if 0
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
    //https://demo.thingsboard.io/api/v1/6osoIZrRH5ShhmwRK6Aj/telemetry
    httplib::SSLClient cli("demo.thingsboard.io"); //服务器地址需要去掉https:// ，否则无法连接到服务器，可先尝试是否可以ping通
  // httplib::SSLClient cli("localhost", 8080);
  // httplib::SSLClient cli("google.com");
  //httplib::SSLClient cli("www.baidu.com");
  cli.set_ca_cert_path(CA_CERT_FILE);
  cli.enable_server_certificate_verification(true); //如果认证失败可设置为false
#else
  httplib::Client cli("localhost", 8080);
#endif

  auto res = cli.Post("/api/v1/6osoIZrRH5ShhmwRK6Aj/telemetry",  //提交路径
                    "{\n"
                    "    \"Dev_ID\": \"76871d60-b7b6-11ec-9a68-6b50da95566e\",\n"
                    "    \"Dev_Nm\": \"Qh-220401\",\n"
                    "    \"D_X\":108.6780,\n"
                    "    \"D_Y\":1.29,\n"
                    "    \"Veh_Heading\":0.0000,\n"
                    "    \"Eng_On\": 1,\n"
                    "    \"Batt_Lvl\": 189,\n"
                    "    \"Alm_Lvl\": 0,\n"
                    "    \"GPS_Time\": 1333026618\n"
                    "}\n",
                    "application/json");

  if (res) {
    if (res->status == 200) {
      std::cout << res->body << std::endl;
    }
  } else {
    std::cout << "post failed" << std::endl;
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
    auto result = cli.get_openssl_verify_result();
    if (result) {
      std::cout << "verify error: " << X509_verify_cert_error_string(result) << std::endl;
    }
#endif
    auto err = res.error();
    std::cout << err << std::endl;
  }
#endif
  JSON_BODY_TELEMETRY_DATA_S body;
  strcpy(body.dev_id, "76871d60-b7b6-11ec-9a68-6b50da95566e");
  strcpy(body.dev_num, "Qh-220401");
  body.d_x = 123.567;
  body.d_y = 100.123;
  body.veh_heading = 0.0001;
  body.eng_on = 1;
  body.batt_lvl = 100;
  body.alm_lvl = 1;
  body.gps_time = 1333026618;
  AddJsonBody(TELEMETRY_DATA, &body, sizeof (body));
  size_t bodys_size;
  JSON_BODY_BUFFER_S buffer;
  {
      bodys_size = m_jsonBodys.size();
  }

  if(bodys_size > 0)
  {
      JSON_BODY_BUFFER_S *pGetBody = NULL;
      {
          pGetBody = m_jsonBodys.front();
      }
      switch (pGetBody->body_type) {
      case TELEMETRY_DATA:
      {
          JSON_BODY_TELEMETRY_DATA_S *pBody = (JSON_BODY_TELEMETRY_DATA_S*)pGetBody->data;
          snprintf(buffer.data, sizeof (buffer.data),
                  "{\n"
                  "    \"Dev_ID\": \"%s\",\n"
                  "    \"Dev_Nm\": \"%s\",\n"
                  "    \"D_X\":%f,\n"
                  "    \"D_Y\":%f,\n"
                  "    \"Veh_Heading\":%f,\n"
                  "    \"Eng_On\": %d,\n"
                  "    \"Batt_Lvl\": %d,\n"
                  "    \"Alm_Lvl\": %d,\n"
                  "    \"GPS_Time\": %ld\n"
                  "}\n",
                   pBody->dev_id, pBody->dev_num, pBody->d_x, pBody->d_y,
                   pBody->veh_heading, pBody->eng_on, pBody->batt_lvl,
                   pBody->alm_lvl, pBody->gps_time);
      }
          break;
      default:
          printf("Unknow Json body\n");
          break;
      }
      if(PostJsonBody(buffer.data) == 0)
      {
          printf("Post Json body succeed\n");
          list<JSON_BODY_BUFFER_S*>::iterator it;
          for(it = m_jsonBodys.begin(); it != m_jsonBodys.end(); ++it)
          {
              if((*it)->id == pGetBody->id)
              {
                  m_jsonBodys.erase(it);
                  free(pGetBody);
                  pGetBody = NULL;
                  break;
              }
          }
      }
      else
      {
          printf("Post Json body failed!\n");
      }
  }
      return 0;
}
