#include "utils.hpp"
#include "OtpMigration.pb.h"

#include <cassert>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <string>

#include <uriparser/Uri.h>
#include <uriparser/UriBase.h>
#include <nlohmann/json.hpp>

#include <Magick++.h>
#include <zbar.h>

#define TOPT_CLOCK_TYPE "period"
#define HOPT_CLOCK_TYPE "counter"

std::string ScanImage(const std::filesystem::path& path);
std::string GetDataFromURL(const std::string& url);

int main(const int argc, const char *argv[])
{
  assert(argc == 2);
  std::filesystem::path path = argv[1];
  
  auto url = ScanImage(path);
  auto rawData = GetDataFromURL(url);
  auto decodedData = Base64Decode(rawData);

  MigrationPayload mPayload;
  mPayload.ParseFromArray(decodedData.data(), decodedData.size());

  nlohmann::json jsonData;
  for (auto p : mPayload.otp_parameters())
  {
    const char* clock_type = p.type() == HOTP ? TOPT_CLOCK_TYPE : HOPT_CLOCK_TYPE;
    jsonData.push_back({
      {"label", p.name()},
      {"issuer", p.issuer()},
      {"type", OtpType_Name(p.type())},
      {"digits", p.digits()},
      {clock_type, p.counter()},
      {"algorithm", Algorithm_Name(p.algorithm())},
      {"secret", Base32Encode(p.secret())}
    });
  }

  std::cout << jsonData.dump(2) << std::endl;
  
  return 0;
}

std::string ScanImage(const std::filesystem::path& path)
{
  // create a reader
  zbar::ImageScanner scanner;
  // configure the reader
  scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);

  Magick::Image magick(path);
  int width = magick.columns();   // extract dimensions
  int height = magick.rows();
  Magick::Blob blob;              // extract the raw data
  magick.modifyImage();
  magick.write(&blob, "GRAY", 8);
  const void *raw = blob.data();

  // wrap image data
  zbar::Image image(width, height, "Y800", raw, width * height);

  // scan the image for barcodes
  assert(scanner.scan(image) == 1);

  return image.symbol_begin()->get_data();
}

std::string GetDataFromURL(const std::string& url)
{
  std::string data;
  UriUriA uri;
  const char* errorPos;
  UriQueryListA* queryList;
  int itemCount;
  
  assert(uriParseSingleUriA(&uri, url.c_str(), &errorPos) == URI_SUCCESS);
  assert(uriDissectQueryMallocA(&queryList, &itemCount, uri.query.first, uri.query.afterLast) == URI_SUCCESS);

  for (int i = 0; i < itemCount; i++)
  {
    if (std::strcmp(queryList[i].key, "data") == 0)
    {
      data = queryList[i].value;
      break;
    }
  }

  uriFreeQueryListA(queryList);
  return data;
}

// struct ArgumentOption
// {
//   std::string long_;
//   char short_;
//   std::string description;
//   bool optional;
// };

  // ArgumentOption argumentOptions[] = {
  //   {"help", 'h', "Show help(this).", true},
  //   {"json", 'j', "Export in JSON format.", true},
  //   {"pretty", 'p', "Export in JSON format.", true},
  // };
