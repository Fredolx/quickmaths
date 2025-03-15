import 'dart:ffi';

import 'package:ffi/ffi.dart';
import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:loader_overlay/loader_overlay.dart';

main() async {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Quick Maths',
      theme: ThemeData(
        // This is the theme of your application.
        //
        // TRY THIS: Try running your application with "flutter run". You'll see
        // the application has a purple toolbar. Then, without quitting the app,
        // try changing the seedColor in the colorScheme below to Colors.green
        // and then invoke "hot reload" (save your changes or press the "hot
        // reload" button in a Flutter-supported IDE, or press "r" if you used
        // the command line to start the app).
        //
        // Notice that the counter didn't reset back to zero; the application
        // state is not lost during the reload. To reset the state, use hot
        // restart instead.
        //
        // This works for code too, not just values: Most code changes can be
        // tested with just a hot reload.
        colorScheme: ColorScheme.fromSeed(seedColor: Colors.deepPurple),
      ),
      home: const MyHomePage(title: 'Quick Maths - AI equations'),
    );
  }
}

class MyHomePage extends StatefulWidget {
  const MyHomePage({super.key, required this.title});
  final String title;

  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

base class StringDouble extends Struct {
  external Pointer<Utf8> text;
  @Double()
  external double value;
}

typedef GetResultC = StringDouble Function(Pointer<Utf8> str);
typedef GetResult = StringDouble Function(Pointer<Utf8> str);
typedef InitializeC = Void Function();
typedef Initialize = void Function();
final dylib = DynamicLibrary.open('libquickmaths.so');
final Initialize init = dylib.lookupFunction<InitializeC, Initialize>(
  'Initialize',
);
final GetResult getResult = dylib.lookupFunction<GetResultC, GetResult>(
  'GetResult',
);
StringDouble getResultWork(String query) {
  var queryNative = query.toNativeUtf8();
  var result = getResult(queryNative);
  calloc.free(queryNative);
  return result;
}

class _MyHomePageState extends State<MyHomePage> {
  double? lastResult;
  String? lastEquation;
  String? lastPrompt;
  final TextEditingController _controller = TextEditingController();
  @override
  void initState() {
    super.initState();
    init();
    print("state initialized");
  }

  calculate() async {
    var query = _controller.text;
    setState(() {
      lastPrompt = query;
      _controller.clear();
    });
    context.loaderOverlay.show();
    var result = await compute(getResultWork, query);
    lastResult = result.value;
    lastEquation = result.text.toDartString();
    calloc.free(result.text);
    setState(() {
      lastResult;
    });
    context.loaderOverlay.hide();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        title: Text(widget.title),
      ),
      body: LoaderOverlay(
        child: Padding(
          padding: EdgeInsets.fromLTRB(30, 200, 30, 0),
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              TextField(
                controller: _controller,
                onSubmitted: (_) async => calculate(),
                decoration: InputDecoration(
                  hintText: "Try typing '% increase of 10 to 30'",
                ),
              ),
              SizedBox(height: 30),
              Text(
                "Prompt: ${lastPrompt ?? ""}\nEquation: ${lastEquation ?? ""}\nResult: ${lastResult ?? ""}",
                style: TextStyle(fontSize: 16),
              ),
            ],
          ),
        ),
      ),
    );
  }
}
