import 'dart:io';
import 'package:dio/dio.dart';
import 'package:path_provider/path_provider.dart';

class ExternalAssetsHandler {
  static Future<void> downloadAssets() async {
    Dio dio = Dio();

    // List of filenames to download
    List<String> files = [
      "added_tokens.json",
      "configuration_phi3.py",
      "genai_config.json",
      "merges.txt",
      "model.onnx",
      "model.onnx.data",
      "tokenizer.json",
      "tokenizer_config.json",
      "vocab.json",
    ];

    Directory dir = await getApplicationSupportDirectory();
    String savePath = "${dir.path}/model";
    print(savePath);
    Directory(savePath).createSync(recursive: true);

    // String baseUrl =
    //     "https://huggingface.co/microsoft/Phi-4-mini-instruct-onnx/resolve/main/cpu_and_mobile/cpu-int4-rtn-block-32-acc-level-4/";
    String baseUrl = "http://192.168.0.150:7878/";

    for (String file in files) {
      String url = "$baseUrl$file";
      String filePath = "$savePath/$file";

      try {
        print("Downloading $file...");
        await dio.download(url, filePath);
        print("$file downloaded successfully.");
      } catch (e) {
        print("Error downloading $file: $e");
      }
    }

    print("All files downloaded to: $savePath");
  }
}
