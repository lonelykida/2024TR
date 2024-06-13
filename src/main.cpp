/* Authors: Lutong Wang and Bangqi Xu */
/*
 * Copyright (c) 2019, The Regents of the University of California
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the University nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string>
#include <chrono>
#include "FlexRoute.h"

using namespace std;
using namespace fr;

int readParams(const string &fileName) {  //读取参数到global.cpp
  //目的是解析一个参数文件，其中每一行都包含一个键值对，键和值之间用冒号（:）分隔
  int readParamCnt = 0;
  fstream fin(fileName.c_str());
  string line;

  if (fin.is_open()){
    while (fin.good()){
      getline(fin, line); //从文件中读取一行文本到line变量
      if (line[0] != '#'){//判断当前行是否是注释（以#开头的行通常被视为注释）
        char delimiter=':'; //定义分隔键和值的字符
        int pos = line.find(delimiter); //在line字符串中查找分隔符':'的位置
        string field = line.substr(0, pos); //提取从字符串开头到分隔符之前的部分作为字段（键）
        //提取分隔符之后的部分作为值。pos + 1表示分隔符之后的第一个字符的索引，substr函数从这个索引开始取到字符串的末尾，得到值的部分
        string value = line.substr(pos + 1);
        stringstream ss(value);// 创建一个stringstream对象，用于将value字符串转换为其他类型的变量
        //将value字符串转换为适当的数据类型，并赋值给相应的全局变量(定义在global.cpp中)
        if (field == "lef")           { LEF_FILE = value; ++readParamCnt;}
        else if (field == "def")      { DEF_FILE = value; REF_OUT_FILE = DEF_FILE; ++readParamCnt;}
        else if (field == "guide")    { GUIDE_FILE = value; ++readParamCnt;}
        else if (field == "outputTA") { OUTTA_FILE = value; ++readParamCnt;}
        else if (field == "output")   { OUT_FILE = value; ++readParamCnt;}
        else if (field == "outputguide") { OUTGUIDE_FILE = value; ++readParamCnt;}
        else if (field == "outputMaze") { OUT_MAZE_FILE = value; ++readParamCnt;}
        else if (field == "outputDRC") { DRC_RPT_FILE = value; ++readParamCnt;}
        else if (field == "outputCMap") { CMAP_FILE = value; ++readParamCnt;}
        else if (field == "threads")  { MAX_THREADS = atoi(value.c_str()); ++readParamCnt;}
        //VERBOSE是一个全局变量，用于控制程序的详细输出或调试信息的级别
        //例如，如果用户在命令行中包含 -verbose 1，那么VERBOSE将被设置为1，程序将进入详细模式，输出额外的日志信息
        else if (field == "verbose")    VERBOSE = atoi(value.c_str());
        else if (field == "dbProcessNode") { DBPROCESSNODE = value; ++readParamCnt;}
        else if (field == "drouteOnGridOnlyPrefWireBottomLayerNum") { ONGRIDONLY_WIRE_PREF_BOTTOMLAYERNUM = atoi(value.c_str()); ++readParamCnt;}
        else if (field == "drouteOnGridOnlyPrefWireTopLayerNum") { ONGRIDONLY_WIRE_PREF_TOPLAYERNUM = atoi(value.c_str()); ++readParamCnt;}
        else if (field == "drouteOnGridOnlyNonPrefWireBottomLayerNum") { ONGRIDONLY_WIRE_NONPREF_BOTTOMLAYERNUM = atoi(value.c_str()); ++readParamCnt;}
        else if (field == "drouteOnGridOnlyNonPrefWireTopLayerNum") { ONGRIDONLY_WIRE_NONPREF_TOPLAYERNUM = atoi(value.c_str()); ++readParamCnt;}
        else if (field == "drouteOnGridOnlyViaBottomLayerNum") { ONGRIDONLY_VIA_BOTTOMLAYERNUM = atoi(value.c_str()); ++readParamCnt;}
        else if (field == "drouteOnGridOnlyViaTopLayerNum") { ONGRIDONLY_VIA_TOPLAYERNUM = atoi(value.c_str()); ++readParamCnt;}
        else if (field == "drouteViaInPinBottomLayerNum") { VIAINPIN_BOTTOMLAYERNUM = atoi(value.c_str()); ++readParamCnt;}
        else if (field == "drouteViaInPinTopLayerNum") { VIAINPIN_TOPLAYERNUM = atoi(value.c_str()); ++readParamCnt;}
        else if (field == "drouteEndIterNum") { END_ITERATION = atoi(value.c_str()); ++readParamCnt;}
        else if (field == "OR_SEED") {OR_SEED = atoi(value.c_str()); ++readParamCnt;}
        else if (field == "OR_K") {OR_K = atof(value.c_str()); ++readParamCnt;}
      }
    }
    
    fin.close();
  }
  
  
  if (readParamCnt < 5) { //读取的参数小于5个返回2
    return 2;
  } else {
    return 0;
  }
}

int main(int argc, char** argv) {

  //cout <<CPX_MIN <<endl;
  //return 0;
  using namespace std::chrono;
  high_resolution_clock::time_point t1 = high_resolution_clock::now();  //记录程序开始执行的时间点
  //double startTime = omp_get_wtime();

  //std::ios::sync_with_stdio(false);
  //cout <<"TritonRoute Version 0.0.6.0" <<endl;
  //cout <<"Developed by Lutong Wang and Bangqi Xu\n"
  //     <<"\n"
  //     <<"Copyright (c) 2019, The Regents of the University of California\n"
  //     <<"All rights reserved.\n"
  //     <<"\n"
  //     <<"Redistribution and use in source and binary forms, with or without\n"
  //     <<"modification, are permitted provided that the following conditions are met:\n"
  //     <<"    * Redistributions of source code must retain the above copyright\n"
  //     <<"      notice, this list of conditions and the following disclaimer.\n"
  //     <<"    * Redistributions in binary form must reproduce the above copyright\n"
  //     <<"      notice, this list of conditions and the following disclaimer in the\n"
  //     <<"      documentation and/or other materials provided with the distribution.\n"
  //     <<"    * Neither the name of the University nor the\n"
  //     <<"      names of its contributors may be used to endorse or promote products\n"
  //     <<"      derived from this software without specific prior written permission.\n"
  //     <<"\n"
  //     <<"THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND\n"
  //     <<"ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED\n"
  //     <<"WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE\n"
  //     <<"DISCLAIMED. IN NO EVENT SHALL THE REGENTS BE LIABLE FOR ANY\n"
  //     <<"DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES\n"
  //     <<"(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;\n"
  //     <<"LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND\n"
  //     <<"ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT\n"
  //     <<"(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS\n"
  //     <<"SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n"
  //     <<"\n";
  if (argc == 1) {//检查命令行参数的数量，并在参数数量不正确时提供使用说明和错误信息
    cout <<"Error: usage ./TritonRoute -lef <LEF_FILE> -def <DEF_FILE> -guide <GUIDE_FILE> -output <OUTPUT_DEF>" <<endl;
    return 2;
  }

  //处理命令行参数，并根据参数执行相应的操作。代码分为两个主要部分：处理参数文件和处理命令行选项
  if (argc == 2) {  //如果只有一个参数，则读取参数文件（处理参数文件）
    int readSuccess = readParams(string(argv[1]));
    if (readSuccess) {
      cout <<"Error reading param file!!!" <<endl;
      return 2;
    }
  } else {  //如果有多个参数，则从命令行参数中读取（处理命令行选项）
    argv++; //argv和argc分别递增和递减，以跳过程序名称，从第一个实际参数开始处理
    argc--;
    while (argc--) {  //循环遍历所有剩余的命令行参数
      if (strcmp(*argv, "-lef") == 0) { //使用strcmp函数检查它是否匹配特定的选项（如"-lef", "-def", "-guide"等）
        argv++; //如果参数匹配，程序会递增argv和递减argc，并将相应的值赋给全局变量（如LEF_FILE, DEF_FILE, GUIDE_FILE等）
        argc--;
        LEF_FILE = *argv;
        //cout <<"lef: " <<LEF_FILE <<endl;
      } else if (strcmp(*argv, "-def") == 0) {
        argv++;
        argc--;
        DEF_FILE = *argv;
        REF_OUT_FILE = DEF_FILE; 
        //cout <<"def: " <<DEF_FILE <<endl;
      } else if (strcmp(*argv, "-guide") == 0) {
        argv++;
        argc--;
        GUIDE_FILE = *argv;
        //cout <<"guide: " <<GUIDE_FILE <<endl;
      } else if (strcmp(*argv, "-threads") == 0) {
        argv++;
        argc--;
        sscanf(*argv, "%d", &MAX_THREADS);  //对于需要转换为数值的参数（如"-threads"和"-verbose"），使用sscanf和atoi函数进行转换
        //cout <<"thread: " <<MAX_THREADS <<endl;
      } else if (strcmp(*argv, "-output") == 0) {
        argv++;
        argc--;
        OUT_FILE = *argv;
        //cout <<"output: " <<OUT_FILE <<endl;
      } else if (strcmp(*argv, "-verbose") == 0) {
        argv++;
        argc--;
        VERBOSE = atoi(*argv);
        //cout <<"output: " <<OUT_FILE <<endl;
      } else {//如果遇到不识别的选项，程序打印错误消息 "ERROR: Illegal command line option: " 后跟非法选项的内容，并返回状态码2
        cout <<"ERROR: Illegal command line option: " <<*argv <<endl;
        return 2;
      }
      argv++;
    }
  }
  

  //开始布线
  FlexRoute router; //实例化FlexRoute类
  router.main();
  
  //记录程序结束的时间点
  high_resolution_clock::time_point t2 = high_resolution_clock::now();
  //计算执行时间
  duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
  /*
    duration: std::chrono中的一个函数模板，用于将一个时间段（duration）转换为另一个时间段，这里指定转换为目标类型duration<double>
    t2和t1是两个high_resolution_clock::time_point对象，代表了两个不同的时间点。减法操作会返回一个duration对象，表示这两个时间点之间的时间差
    默认情况下t2 - t1得到的duration对象可能不是以double为单位的，而是以high_resolution_clock的内部时钟周期为单位
    通过duration_cast转换，可以确保得到的时间段是以秒为单位的，便于以人类可读的格式显示或记录
  */
  if (VERBOSE > 0) {//如果VERBOSE标志设置为大于0的值，则将运行时间打印到控制台
    //程序检查VERBOSE的值，如果大于0，则输出程序的运行时间。这是一种常见的做法，用于在详细模式下提供额外的运行时信息
    cout <<endl <<"Runtime taken (hrt): " << time_span.count()    <<endl;
  }
  return 0;
}
