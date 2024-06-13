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

#ifndef _FR_VIADEF_H_
#define _FR_VIADEF_H_

#include <vector>
#include <memory>
#include "frBaseTypes.h"
#include "db/obj/frShape.h"

namespace fr{ //fr命名空间 - 定义了frLef58Cut类和frViaDef类
  //frLef5.8参考手册中的Cut类
  class frLef58CutClass { 
  public:
    // constructors
    //构造，默认name="",Cut宽度viaWidth=0,Cut长度viaLength=0,Cut层数numCut=1
    frLef58CutClass(): name(""), viaWidth(0), viaLength(0), numCut(1) {}
    // getters - 获取器 - 给成员变量name拷贝到in
    void getName(std::string &in) const {
      in = name;
    }
    //直接获取name
    std::string getName() const { 
      return name;
    }
    //获取Cut宽度viaWidth
    frCoord getViaWidth() const { 
      return viaWidth;
    }
    //判断viaLength == viaWidth
    bool hasViaLength() const {   
      return (viaLength == viaWidth) ? false : true;
    }
    //获取viaLength
    frCoord getViaLength() const {
      return viaLength;
    }
    //bool hasNumCut() const {
    //  return hNumCut;
    //}
    //获取Cut层数numCut
    frUInt4 getNumCut() const {
      return numCut;
    }
    // setters - 设置器 - 将in拷贝到成员变量name
    void setName(frString &in) {
      name = in;
    }
    //设置Cut宽度viaWidth=in
    void setViaWidth(frCoord in) {
      viaWidth = in;
    }
    //设置Cut长度viaLength=in
    void setViaLength(frCoord in) {
      //hViaLength = true;
      viaLength = in;
    }
    //设置Cut数numCut=in
    void setNumCut(frUInt4 in) {
      numCut = in;
    }
  protected:
    std::string name; // Cut名
    frCoord  viaWidth;// Cut宽度
    //bool     hViaLength;  
    frCoord  viaLength; // Cut长度
    frUInt4  numCut; // Cut层数 - this value is not equal to #multi cuts, only used for calculating resistance, currently ignored in rule checking process
  };

  //fr通孔定义
  class frViaDef {
  public:
    // constructors
    //无参构造函数，默认name="",isDefault=false,layer1Figs={},layer2Figs={},cutFigs={},cutClass=nullptr
    frViaDef(): name(), isDefault(false), layer1Figs(), layer2Figs(), cutFigs(), cutClass(nullptr), cutClassIdx(-1) {}
    
    //frViaDef(const frViaDef &in): name(defIn.name), isDefault(defIn.isDefault), cutClass(defIn.cutClass) {}
    //指定的有参构造 - nameIn，其它同无参构造
    frViaDef(const std::string &nameIn): name(nameIn), isDefault(false), layer1Figs(),
                                     layer2Figs(), cutFigs(), cutClass(nullptr), cutClassIdx(-1) {}
    // getters - 获取器
    //将成员变量name拷贝到nameIn
    void getName(std::string &nameIn) const { 
      nameIn = name;
    }
    //直接获取name
    std::string getName() const {
      return name;
    }
    //获取Layer1层的编号
    frLayerNum getLayer1Num() const {
      if (layer1Figs.size()) {  //该通孔在layer1层上有形状
        return (layer1Figs.at(0))->getLayerNum();
      } else {
        std::cout <<"Error: via does not have shape on layer 1" <<std::endl;
        exit(1);
      }
    }
    //获取Layer2层的编号
    frLayerNum getLayer2Num() const {
      if (layer2Figs.size()) {  //该通孔在layer2层上有形状
        return (layer2Figs.at(0))->getLayerNum();
      } else {
        std::cout <<"Error: via does not have shape on layer 2" <<std::endl;
        exit(1);
      }
    }
    //获取Cut层的编号
    frLayerNum getCutLayerNum() const {
      if (cutFigs.size()) { //该通孔在cut层上有形状
        return (cutFigs.at(0))->getLayerNum();
      } else {
        std::cout <<"Error: via does not have shape on layer cut" <<std::endl;
        exit(1);
      }
    }
    //获取Layer1层的形状
    std::vector<std::unique_ptr<frShape> >& getLayer1Figs() {
      return layer1Figs;
    }
    //获取Layer2层的形状
    std::vector<std::unique_ptr<frShape> >& getLayer2Figs() {
      return layer2Figs;
    }
    //获取Cut层的形状
    std::vector<std::unique_ptr<frShape> >& getCutFigs() {
      return cutFigs;
    }
    //获取layer1层的const形状
    const std::vector<std::unique_ptr<frShape> >& getLayer1Figs() const {
      return layer1Figs;
    }
    //获取layer2层的const形状
    const std::vector<std::unique_ptr<frShape> >& getLayer2Figs() const {
      return layer2Figs;
    }
    //获取cut层的const形状
    const std::vector<std::unique_ptr<frShape> >& getCutFigs() const {
      return cutFigs;
    }
    //看是否是默认Cut，默认是false
    bool getDefault() const { 
      return isDefault;
    }
    //获取Cut数
    int getNumCut() const {
      return cutFigs.size();
    }
    //看是否有Cut类指针
    bool hasCutClass() const {
      return (cutClass != nullptr);
    }
    //获取Cut类指针
    frLef58CutClass* getCutClass() const {
      return cutClass;
    }
    //获取Cut类索引
    int getCutClassIdx() const {
      return cutClassIdx;
    }
    //看是否已经被Router添加
    bool isAddedByRouter() const {
      return addedByRouter;
    }
    //看是否有多个Cut
    bool isMultiCut() const {
      return (cutFigs.size() > 1) ? true : false;
    }
    //void getLayer1BBox (frBox &boxIn) const;
    //void getCutBBox (frBox &boxIn) const;
    //void getLayer2BBox (frBox &boxIn) const;
    // setters - 设置器
    //添加Layer1层形状
    void addLayer1Fig(std::unique_ptr<frShape> &figIn) {
      layer1Figs.push_back(std::move(figIn));
    }
    //添加Layer2层形状
    void addLayer2Fig(std::unique_ptr<frShape> &figIn) {
      layer2Figs.push_back(std::move(figIn));
    }
    //添加Cut层形状
    void addCutFig(std::unique_ptr<frShape> &figIn) {
      cutFigs.push_back(std::move(figIn));
    }
    //设置默认Cut
    void setDefault(bool isDefaultIn) {
      isDefault = isDefaultIn;
    }
    //设置Cut类
    void setCutClass(frLef58CutClass *in) {
      cutClass = in;
    }
    //设置Cut类索引
    void setCutClassIdx(int in) {
      cutClassIdx = in;
    }
    //设置已经被Router添加
    void setAddedByRouter(bool in) {
      addedByRouter = in;
    }
  protected:
    std::string                                 name;
    bool                                     isDefault; //默认false
    std::vector< std::unique_ptr<frShape> >  layer1Figs;//layer1层形状
    std::vector< std::unique_ptr<frShape> >  layer2Figs;//layer2层形状
    std::vector< std::unique_ptr<frShape> >  cutFigs;   //cut层形状
    frLef58CutClass*                         cutClass;  //Cut类指针
    int                                      cutClassIdx;//Cut类索引
    bool                                     addedByRouter;//是否已经被Router添加
  };
}
#endif
