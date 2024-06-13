//实现了frRegionQuery类的功能，
//这个类可能用于查询设计中的区域信息，例如查找与特定区域重叠的引脚、障碍物或路由对象

/*
`frRegionQuery.cpp` 文件的作用是实现了一个区域查询系统，它是电子设计自动化（EDA）中
用于集成电路设计和布局的一部分。这个系统允许用户查询给定区域内的电路设计对象，如引脚（pins）、
宏单元（macros）、导线（guides）等。以下是该文件的主要功能和作用：
  1. 形状管理：文件中定义了函数来添加和移除各种形状（如 `frShape`、`frVia`、`frInstTerm` 等）到区域查询数据结构中。
  2. 区域查询：实现了查询操作，如 `query`、`queryGuide`、`queryRPin`、`queryDRObj` 和 `queryGRObj` 等，这些函数用于搜索与给定框（`frBox`）相交的电路设计对象。
  3. 初始化：包含了初始化区域查询系统的功能，如 `init`、`initOrigGuide`、`initGuide`、`initGRPin`、`initRPin`、`initDRObj` 和 `initGRObj` 等函数，这些函数用于设置查询结构。
  4. 打印和调试：提供了 `print`、`printGuide`、`printDRObj` 和 `printGRObj` 等函数，用于打印当前区域查询的状态，帮助开发者了解系统内部的状态和调试问题。
  5. 数据结构管理：实现了对内部数据结构的管理，如 `clearGuides` 函数，它清除了导线数据。
  6. 设计规则检查（DRC）：通过区域查询来辅助设计规则检查，确保设计满足特定的工艺要求。
  7. 交互式设计和优化：区域查询是设计交互、优化和修改过程中的一个重要组成部分，允许设计师查询和分析设计空间。
  8. 集成 Boost 库：使用了 Boost 库中的 `adjacency_list` 和 `geometry` 组件，来提供高效的图形表示和几何计算功能。
总的来说，`frRegionQuery.cpp` 是 EDA 工具集中的一个关键组件，它提供了一套丰富的功能来处理和查询集成电路设计中的各种对象，对于设计验证、优化和制造前的准备工作至关重要。
*/

#include <iostream>
#include "global.h"
#include "frDesign.h"
#include "frRegionQuery.h"
using namespace std;
using namespace fr;

// frRegionQuery 类的成员函数，用于添加形状对象到区域查询中
void frRegionQuery::add(frShape* shape) {
  frBox frb;  // 声明 frBox 类型的变量 frb，用于存储形状的边界框
  box_t boostb;// 声明 box_t 类型的变量 boostb，用于存储 Boost 库使用的边界框
  // 检查形状类型是否为 frcPathSeg 或 frcRect
  if (shape->typeId() == frcPathSeg || shape->typeId() == frcRect) {
    shape->getBBox(frb);// 获取形状的边界框
    // 将 frBox 转换为 Boost 库使用的边界框格式
    boostb = box_t(point_t(frb.left(), frb.bottom()), point_t(frb.right(), frb.top()));
    // 将形状及其边界框添加到对应层级的容器中
    shapes.at(shape->getLayerNum()).insert(make_pair(boostb, shape));
  } else {// 如果形状类型不支持，则输出错误信息
    cout <<"Error: unsupported region query add" <<endl;
  }
}
// 接下来的代码包含了多个重载版本的 add 函数和其他相关函数，如 addDRObj、addMarker、addVia 等。
// 这些函数的实现与第一个 add 函数类似，用于不同类型的对象添加到区域查询中。

inline void frRegionQuery::add(frShape* shape, vector<vector<rq_rptr_value_t<frBlockObject> > > &allShapes) {
  frBox frb;
  box_t boostb;
  if (shape->typeId() == frcPathSeg || shape->typeId() == frcRect) {
    shape->getBBox(frb);
    boostb = box_t(point_t(frb.left(), frb.bottom()), point_t(frb.right(), frb.top()));
    allShapes.at(shape->getLayerNum()).push_back(make_pair(boostb, shape));
  } else {
    cout <<"Error: unsupported region query add" <<endl;
  }
}

void frRegionQuery::addDRObj(frShape* shape) {
  frBox frb;
  box_t boostb;
  if (shape->typeId() == frcPathSeg || shape->typeId() == frcRect || shape->typeId() == frcPatchWire) {
    shape->getBBox(frb);
    boostb = box_t(point_t(frb.left(), frb.bottom()), point_t(frb.right(), frb.top()));
    drObjs.at(shape->getLayerNum()).insert(make_pair(boostb, shape));
  } else {
    cout <<"Error: unsupported region query add" <<endl;
  }
}

void frRegionQuery::addMarker(frMarker* in) {
  frBox frb;
  box_t boostb;
  in->getBBox(frb);
  boostb = box_t(point_t(frb.left(), frb.bottom()), point_t(frb.right(), frb.top()));
  markers.at(in->getLayerNum()).insert(make_pair(boostb, in));
}

inline void frRegionQuery::addDRObj(frShape* shape, vector<vector<rq_rptr_value_t<frBlockObject> > > &allShapes) {
  frBox frb;
  box_t boostb;
  if (shape->typeId() == frcPathSeg || shape->typeId() == frcRect || shape->typeId() == frcPatchWire) {
    shape->getBBox(frb);
    boostb = box_t(point_t(frb.left(), frb.bottom()), point_t(frb.right(), frb.top()));
    allShapes.at(shape->getLayerNum()).push_back(make_pair(boostb, shape));
  } else {
    cout <<"Error: unsupported region query add" <<endl;
  }
}

void frRegionQuery::removeDRObj(frShape* shape) {
  frBox frb;
  box_t boostb;
  if (shape->typeId() == frcPathSeg || shape->typeId() == frcRect || shape->typeId() == frcPatchWire) {
    shape->getBBox(frb);
    boostb = box_t(point_t(frb.left(), frb.bottom()), point_t(frb.right(), frb.top()));
    drObjs.at(shape->getLayerNum()).remove(make_pair(boostb, shape));
  } else {
    cout <<"Error: unsupported region query add" <<endl;
  }
}

void frRegionQuery::addGRObj(grShape* shape) {
  frBox frb;
  box_t boostb;
  if (shape->typeId() == grcPathSeg) {
    shape->getBBox(frb);
    boostb = box_t(point_t(frb.left(), frb.bottom()), point_t(frb.right(), frb.top()));
    grObjs.at(shape->getLayerNum()).insert(make_pair(boostb, shape));
  } else {
    cout <<"Error: unsupported region query add" <<endl;
  }
}

inline void frRegionQuery::addGRObj(grShape* shape, vector<vector<rq_rptr_value_t<grBlockObject> > > &allShapes) {
  frBox frb;
  box_t boostb;
  if (shape->typeId() == grcPathSeg) {
    shape->getBBox(frb);
    boostb = box_t(point_t(frb.left(), frb.bottom()), point_t(frb.right(), frb.top()));
    allShapes.at(shape->getLayerNum()).push_back(make_pair(boostb, shape));
  } else {
    cout <<"Error: unsupported region query add" <<endl;
  }
}

void frRegionQuery::removeGRObj(grShape* shape) {
  frBox frb;
  box_t boostb;
  if (shape->typeId() == grcPathSeg) {
    shape->getBBox(frb);
    boostb = box_t(point_t(frb.left(), frb.bottom()), point_t(frb.right(), frb.top()));
    grObjs.at(shape->getLayerNum()).remove(make_pair(boostb, shape));
  } else {
    cout <<"Error: unsupported region query add" <<endl;
  }
}

void frRegionQuery::removeMarker(frMarker* in) {
  frBox frb;
  box_t boostb;
  in->getBBox(frb);
  boostb = box_t(point_t(frb.left(), frb.bottom()), point_t(frb.right(), frb.top()));
  markers.at(in->getLayerNum()).remove(make_pair(boostb, in));
}

void frRegionQuery::add(frVia* via) {
  frBox frb;
  frTransform xform;
  frPoint origin;
  via->getOrigin(origin);
  xform.set(origin);
  box_t boostb;
  for (auto &uShape: via->getViaDef()->getLayer1Figs()) {
    auto shape = uShape.get();
    if (shape->typeId() == frcRect) {
      shape->getBBox(frb);
      frb.transform(xform);
      boostb = box_t(point_t(frb.left(), frb.bottom()), point_t(frb.right(), frb.top()));
      shapes.at(via->getViaDef()->getLayer1Num()).insert(make_pair(boostb, via));
    } else {
      cout <<"Error: unsupported region query add" <<endl;
    }
  }
  for (auto &uShape: via->getViaDef()->getLayer2Figs()) {
    auto shape = uShape.get();
    if (shape->typeId() == frcRect) {
      shape->getBBox(frb);
      frb.transform(xform);
      boostb = box_t(point_t(frb.left(), frb.bottom()), point_t(frb.right(), frb.top()));
      shapes.at(via->getViaDef()->getLayer2Num()).insert(make_pair(boostb, via));
    } else {
      cout <<"Error: unsupported region query add" <<endl;
    }
  }
  for (auto &uShape: via->getViaDef()->getCutFigs()) {
    auto shape = uShape.get();
    if (shape->typeId() == frcRect) {
      shape->getBBox(frb);
      frb.transform(xform);
      boostb = box_t(point_t(frb.left(), frb.bottom()), point_t(frb.right(), frb.top()));
      shapes.at(via->getViaDef()->getCutLayerNum()).insert(make_pair(boostb, via));
    } else {
      cout <<"Error: unsupported region query add" <<endl;
    }
  }
}

inline void frRegionQuery::add(frVia* via, vector<vector<rq_rptr_value_t<frBlockObject> > > &allShapes) {
  frBox frb;
  frTransform xform;
  frPoint origin;
  via->getOrigin(origin);
  xform.set(origin);
  box_t boostb;
  for (auto &uShape: via->getViaDef()->getLayer1Figs()) {
    auto shape = uShape.get();
    if (shape->typeId() == frcRect) {
      shape->getBBox(frb);
      frb.transform(xform);
      boostb = box_t(point_t(frb.left(), frb.bottom()), point_t(frb.right(), frb.top()));
      allShapes.at(via->getViaDef()->getLayer1Num()).push_back(make_pair(boostb, via));
    } else {
      cout <<"Error: unsupported region query add" <<endl;
    }
  }
  for (auto &uShape: via->getViaDef()->getLayer2Figs()) {
    auto shape = uShape.get();
    if (shape->typeId() == frcRect) {
      shape->getBBox(frb);
      frb.transform(xform);
      boostb = box_t(point_t(frb.left(), frb.bottom()), point_t(frb.right(), frb.top()));
      allShapes.at(via->getViaDef()->getLayer2Num()).push_back(make_pair(boostb, via));
    } else {
      cout <<"Error: unsupported region query add" <<endl;
    }
  }
  for (auto &uShape: via->getViaDef()->getCutFigs()) {
    auto shape = uShape.get();
    if (shape->typeId() == frcRect) {
      shape->getBBox(frb);
      frb.transform(xform);
      boostb = box_t(point_t(frb.left(), frb.bottom()), point_t(frb.right(), frb.top()));
      allShapes.at(via->getViaDef()->getCutLayerNum()).push_back(make_pair(boostb, via));
    } else {
      cout <<"Error: unsupported region query add" <<endl;
    }
  }
}

void frRegionQuery::addDRObj(frVia* via) {
  frBox frb;
  via->getBBox(frb);
  box_t boostb(point_t(frb.left(), frb.bottom()), point_t(frb.right(), frb.top()));
  drObjs.at(via->getViaDef()->getCutLayerNum()).insert(make_pair(boostb, via));
}

inline void frRegionQuery::addDRObj(frVia* via, vector<vector<rq_rptr_value_t<frBlockObject> > > &allShapes) {
  frBox frb;
  via->getBBox(frb);
  box_t boostb(point_t(frb.left(), frb.bottom()), point_t(frb.right(), frb.top()));
  allShapes.at(via->getViaDef()->getCutLayerNum()).push_back(make_pair(boostb, via));
}

void frRegionQuery::removeDRObj(frVia* via) {
  frBox frb;
  via->getBBox(frb);
  box_t boostb(point_t(frb.left(), frb.bottom()), point_t(frb.right(), frb.top()));
  drObjs.at(via->getViaDef()->getCutLayerNum()).remove(make_pair(boostb, via));
}

void frRegionQuery::addGRObj(grVia* via) {
  frBox frb;
  via->getBBox(frb);
  box_t boostb(point_t(frb.left(), frb.bottom()), point_t(frb.right(), frb.top()));
  grObjs.at(via->getViaDef()->getCutLayerNum()).insert(make_pair(boostb, via));
}

inline void frRegionQuery::addGRObj(grVia *via, vector<vector<rq_rptr_value_t<grBlockObject> > > &allShapes) {
  frBox frb;
  via->getBBox(frb);
  box_t boostb(point_t(frb.left(), frb.bottom()), point_t(frb.right(), frb.top()));
  allShapes.at(via->getViaDef()->getCutLayerNum()).push_back(make_pair(boostb, via));
}

void frRegionQuery::removeGRObj(grVia* via) {
  frBox frb;
  via->getBBox(frb);
  box_t boostb(point_t(frb.left(), frb.bottom()), point_t(frb.right(), frb.top()));
  grObjs.at(via->getViaDef()->getCutLayerNum()).remove(make_pair(boostb, via));
}

void frRegionQuery::add(frInstTerm* instTerm) {
  frBox frb;
  box_t boostb;

  //frTransform xform;
  //instTerm->getInst()->getTransform(xform);

  //frBox mbox;
  //instTerm->getInst()->getRefBlock()->getBoundaryBBox(mbox);
  //// add origin
  //xform.set(xform.xOffset() + mbox.left(), xform.yOffset() + mbox.bottom());
  //frPoint size(mbox.right(), mbox.top());
  //xform.updateXform(size);
  frTransform xform;
  instTerm->getInst()->getUpdatedXform(xform);

  for (auto &pin: instTerm->getTerm()->getPins()) {
    for (auto &uFig: pin->getFigs()) {
      auto shape = uFig.get();
      if (shape->typeId() == frcPathSeg || shape->typeId() == frcRect) {
        shape->getBBox(frb);
        frb.transform(xform);
        boostb = box_t(point_t(frb.left(), frb.bottom()), point_t(frb.right(), frb.top()));
        shapes.at(static_cast<frShape*>(shape)->getLayerNum()).insert(make_pair(boostb, instTerm));
      } else {
        cout <<"Error: unsupported region query add" <<endl;
      }
    }
  }
}

inline void frRegionQuery::add(frInstTerm* instTerm, vector<vector<rq_rptr_value_t<frBlockObject> > > &allShapes) {
  frBox frb;
  box_t boostb;

  //frTransform xform;
  //instTerm->getInst()->getTransform(xform);

  //instTerm->getInst()->getRefBlock()->getBoundaryBBox(frb);
  //xform.updateXform(frb.upperRight());
  frTransform xform;
  instTerm->getInst()->getUpdatedXform(xform);

  for (auto &pin: instTerm->getTerm()->getPins()) {
    for (auto &uFig: pin->getFigs()) {
      auto shape = uFig.get();
      if (shape->typeId() == frcPathSeg || shape->typeId() == frcRect) {
        shape->getBBox(frb);
        frb.transform(xform);
        boostb = box_t(point_t(frb.left(), frb.bottom()), point_t(frb.right(), frb.top()));
        allShapes.at(static_cast<frShape*>(shape)->getLayerNum()).push_back(make_pair(boostb, instTerm));
      } else {
        cout <<"Error: unsupported region query add" <<endl;
      }
    }
  }
}

void frRegionQuery::add(frTerm* term) {
  frBox frb;
  box_t boostb;
  for (auto &pin: term->getPins()) {
    for (auto &uFig: pin->getFigs()) {
      auto shape = uFig.get();
      if (shape->typeId() == frcPathSeg || shape->typeId() == frcRect) {
        shape->getBBox(frb);
        boostb = box_t(point_t(frb.left(), frb.bottom()), point_t(frb.right(), frb.top()));
        shapes.at(static_cast<frShape*>(shape)->getLayerNum()).insert(make_pair(boostb, term));
      } else {
        cout <<"Error: unsupported region query add" <<endl;
      }
    }
  }
}

inline void frRegionQuery::add(frTerm* term, vector<vector<rq_rptr_value_t<frBlockObject> > > &allShapes) {
  frBox frb;
  box_t boostb;
  for (auto &pin: term->getPins()) {
    for (auto &uFig: pin->getFigs()) {
      auto shape = uFig.get();
      if (shape->typeId() == frcPathSeg || shape->typeId() == frcRect) {
        shape->getBBox(frb);
        boostb = box_t(point_t(frb.left(), frb.bottom()), point_t(frb.right(), frb.top()));
        allShapes.at(static_cast<frShape*>(shape)->getLayerNum()).push_back(make_pair(boostb, term));
      } else {
        cout <<"Error: unsupported region query add" <<endl;
      }
    }
  }
}

void frRegionQuery::add(frInstBlockage* instBlk) {
  frBox frb;
  box_t boostb;

  frTransform xform;
  instBlk->getInst()->getUpdatedXform(xform);
  auto blk = instBlk->getBlockage();
  auto pin = blk->getPin();
  for (auto &uFig: pin->getFigs()) {
    auto shape = uFig.get();
    if (shape->typeId() == frcPathSeg || shape->typeId() == frcRect) {
      shape->getBBox(frb);
      frb.transform(xform);
      boostb = box_t(point_t(frb.left(), frb.bottom()), point_t(frb.right(), frb.top()));
      shapes.at(static_cast<frShape*>(shape)->getLayerNum()).insert(make_pair(boostb, instBlk));
    } else {
      cout <<"Error: unsupported region query add" <<endl;
    }
  }
}

inline void frRegionQuery::add(frInstBlockage* instBlk, vector<vector<rq_rptr_value_t<frBlockObject> > > &allShapes) {
  frBox frb;
  box_t boostb;

  frTransform xform;
  instBlk->getInst()->getUpdatedXform(xform);
  auto blk = instBlk->getBlockage();
  auto pin = blk->getPin();
  for (auto &uFig: pin->getFigs()) {
    auto shape = uFig.get();
    if (shape->typeId() == frcPathSeg || shape->typeId() == frcRect) {
      shape->getBBox(frb);
      frb.transform(xform);
      boostb = box_t(point_t(frb.left(), frb.bottom()), point_t(frb.right(), frb.top()));
      allShapes.at(static_cast<frShape*>(shape)->getLayerNum()).push_back(make_pair(boostb, instBlk));
    } else {
      cout <<"Error: unsupported region query add" <<endl;
    }
  }
}

void frRegionQuery::add(frBlockage* blk) {
  frBox frb;
  box_t boostb;
  auto pin = blk->getPin();
  for (auto &uFig: pin->getFigs()) {
    auto shape = uFig.get();
    if (shape->typeId() == frcPathSeg || shape->typeId() == frcRect) {
      shape->getBBox(frb);
      boostb = box_t(point_t(frb.left(), frb.bottom()), point_t(frb.right(), frb.top()));
      shapes.at(static_cast<frShape*>(shape)->getLayerNum()).insert(make_pair(boostb, blk));
    } else {
      cout <<"Error: unsupported region query add" <<endl;
    }
  }
}

inline void frRegionQuery::add(frBlockage* blk, vector<vector<rq_rptr_value_t<frBlockObject> > > &allShapes) {
  frBox frb;
  box_t boostb;
  auto pin = blk->getPin();
  for (auto &uFig: pin->getFigs()) {
    auto shape = uFig.get();
    if (shape->typeId() == frcPathSeg || shape->typeId() == frcRect) {
      shape->getBBox(frb);
      boostb = box_t(point_t(frb.left(), frb.bottom()), point_t(frb.right(), frb.top()));
      allShapes.at(static_cast<frShape*>(shape)->getLayerNum()).push_back(make_pair(boostb, blk));
    } else {
      cout <<"Error: unsupported region query add" <<endl;
    }
  }
}

void frRegionQuery::addGuide(frGuide* guide) {
  frBox frb;
  box_t boostb;
  guide->getBBox(frb);
  boostb = box_t(point_t(frb.left(), frb.bottom()), point_t(frb.right(), frb.top()));
  for (int i = guide->getBeginLayerNum(); i <= guide->getEndLayerNum(); i++) {
    shapes.at(i).insert(make_pair(boostb, guide));
  }
}

inline void frRegionQuery::addGuide(frGuide* guide, vector<vector<rq_rptr_value_t<frGuide> > > &allShapes) {
  frBox frb;
  box_t boostb;
  guide->getBBox(frb);
  boostb = box_t(point_t(frb.left(), frb.bottom()), point_t(frb.right(), frb.top()));
  for (int i = guide->getBeginLayerNum(); i <= guide->getEndLayerNum(); i++) {
    allShapes.at(i).push_back(make_pair(boostb, guide));
  }
}

void frRegionQuery::addRPin(frRPin *rpin, vector<vector<rq_rptr_value_t<frRPin> > > &allRPins) {
  frBox frb;
  box_t boostb;
  // cout << rpin->getNet()->getName() << endl << flush;
  // if (rpin->getNet()->getName() == string("netMA") && rpin->getFrTerm() && rpin->getFrTerm()->typeId() == frcInstTerm) {
  //   auto instTerm = static_cast<frInstTerm*>(rpin->getFrTerm());
  //   cout << instTerm->getInst()->getName() << "/" << instTerm->getTerm()->getName() << endl << flush;
  // }
  frLayerNum layerNum = rpin->getLayerNum();
  rpin->getBBox(frb);
  // if (rpin->getNet()->getName() == string("net70") && rpin->getFrTerm() && rpin->getFrTerm()->typeId() == frcInstTerm) {
  //   auto instTerm = static_cast<frInstTerm*>(rpin->getFrTerm());
  //   cout << instTerm->getInst()->getName() << "/" << instTerm->getTerm()->getName() << endl;
  //   cout << "  rpin bbox (" << frb.left() << ", " << frb.bottom() << ") - (" << frb.right() << ", " << frb.top() << ")\n";
  //   frPoint origin;
  //   instTerm->getInst()->getOrigin(origin);
  //   cout << "  inst at (" << origin.x() << ", " << origin.y() << ") orient = " << int(instTerm->getInst()->getOrient()) << endl;
  //   frPoint pt;
  //   rpin->getAccessPoint()->getPoint(pt);
  //   cout << "  access pt at (" << pt.x() << ", " << pt.y() << ") on layer " << rpin->getLayerNum() << "\n";
  // }
  boostb = box_t(point_t(frb.left(), frb.bottom()), point_t(frb.right(), frb.top()));
  allRPins.at(layerNum).push_back(make_pair(boostb, rpin));
}

inline void frRegionQuery::addOrigGuide(frNet* net, const frRect &rect, vector<vector<rq_rptr_value_t<frNet> > > &allShapes) {
  frBox frb;
  rect.getBBox(frb);
  box_t boostb;
  boostb = box_t(point_t(frb.left(), frb.bottom()), point_t(frb.right(), frb.top()));
  allShapes.at(rect.getLayerNum()).push_back(make_pair(boostb, net));
}

void frRegionQuery::query(const frBox &box, frLayerNum layerNum, vector<rq_rptr_value_t<frBlockObject> > &result) {
  box_t boostb = box_t(point_t(box.left(), box.bottom()), point_t(box.right(), box.top()));
  shapes.at(layerNum).query(bgi::intersects(boostb), back_inserter(result));
  //transform(temp.begin(), temp.end(), back_inserter(result), [](auto &kv) {return kv.second;});
}

void frRegionQuery::queryGuide(const frBox &box, frLayerNum layerNum, vector<rq_rptr_value_t<frGuide> > &result) {
  box_t boostb = box_t(point_t(box.left(), box.bottom()), point_t(box.right(), box.top()));
  guides.at(layerNum).query(bgi::intersects(boostb), back_inserter(result));
  //transform(temp.begin(), temp.end(), back_inserter(result), [](auto &kv) {return kv.second;});
}

void frRegionQuery::queryGuide(const frBox &box, frLayerNum layerNum, vector<frGuide*> &result) {
  vector<rq_rptr_value_t<frGuide> > temp;
  box_t boostb = box_t(point_t(box.left(), box.bottom()), point_t(box.right(), box.top()));
  guides.at(layerNum).query(bgi::intersects(boostb), back_inserter(temp));
  transform(temp.begin(), temp.end(), back_inserter(result), [](auto &kv) {return kv.second;});
}

void frRegionQuery::queryGuide(const frBox &box, vector<frGuide*> &result) {
  vector<rq_rptr_value_t<frGuide> > temp;
  box_t boostb = box_t(point_t(box.left(), box.bottom()), point_t(box.right(), box.top()));
  for (auto &m: guides) {
    m.query(bgi::intersects(boostb), back_inserter(temp));
  }
  transform(temp.begin(), temp.end(), back_inserter(result), [](auto &kv) {return kv.second;});
}

void frRegionQuery::queryRPin(const frBox &box, frLayerNum layerNum, vector<rq_rptr_value_t<frRPin> > &result) {
  box_t boostb = box_t(point_t(box.left(), box.bottom()), point_t(box.right(), box.top()));
  rpins.at(layerNum).query(bgi::intersects(boostb), back_inserter(result));
}

void frRegionQuery::queryOrigGuide(const frBox &box, frLayerNum layerNum, vector<rq_rptr_value_t<frNet> > &result) {
  box_t boostb = box_t(point_t(box.left(), box.bottom()), point_t(box.right(), box.top()));
  origGuides.at(layerNum).query(bgi::intersects(boostb), back_inserter(result));
  //transform(temp.begin(), temp.end(), back_inserter(result), [](auto &kv) {return kv.second;});
}

void frRegionQuery::queryGRPin(const frBox &box, vector<frBlockObject*> &result) {
  vector<rq_rptr_value_t<frBlockObject> > temp;
  box_t boostb = box_t(point_t(box.left(), box.bottom()), point_t(box.right(), box.top()));
  grPins.query(bgi::intersects(boostb), back_inserter(temp));
  transform(temp.begin(), temp.end(), back_inserter(result), [](auto &kv) {return kv.second;});
}

void frRegionQuery::queryDRObj(const frBox &box, frLayerNum layerNum, vector<rq_rptr_value_t<frBlockObject> > &result) {
  box_t boostb = box_t(point_t(box.left(), box.bottom()), point_t(box.right(), box.top()));
  drObjs.at(layerNum).query(bgi::intersects(boostb), back_inserter(result));
}

void frRegionQuery::queryDRObj(const frBox &box, frLayerNum layerNum, vector<frBlockObject*> &result) {
  vector<rq_rptr_value_t<frBlockObject> > temp;
  box_t boostb = box_t(point_t(box.left(), box.bottom()), point_t(box.right(), box.top()));
  drObjs.at(layerNum).query(bgi::intersects(boostb), back_inserter(temp));
  transform(temp.begin(), temp.end(), back_inserter(result), [](auto &kv) {return kv.second;});
}

void frRegionQuery::queryDRObj(const frBox &box, vector<frBlockObject*> &result) {
  vector<rq_rptr_value_t<frBlockObject> > temp;
  box_t boostb = box_t(point_t(box.left(), box.bottom()), point_t(box.right(), box.top()));
  for (auto &m: drObjs) {
    m.query(bgi::intersects(boostb), back_inserter(temp));
  }
  transform(temp.begin(), temp.end(), back_inserter(result), [](auto &kv) {return kv.second;});
}

void frRegionQuery::queryGRObj(const frBox &box, frLayerNum layerNum, vector<rq_rptr_value_t<grBlockObject> > &result) {
  box_t boostb = box_t(point_t(box.left(), box.bottom()), point_t(box.right(), box.top()));
  grObjs.at(layerNum).query(bgi::intersects(boostb), back_inserter(result));
}

void frRegionQuery::queryGRObj(const frBox &box, frLayerNum layerNum, vector<grBlockObject*> &result) {
  vector<rq_rptr_value_t<grBlockObject> > temp;
  box_t boostb = box_t(point_t(box.left(), box.bottom()), point_t(box.right(), box.top()));
  grObjs.at(layerNum).query(bgi::intersects(boostb), back_inserter(temp));
  transform(temp.begin(), temp.end(), back_inserter(result), [](auto &kv) {return kv.second;});
}

void frRegionQuery::queryGRObj(const frBox &box, vector<grBlockObject*> &result) {
  vector<rq_rptr_value_t<grBlockObject> > temp;
  box_t boostb = box_t(point_t(box.left(), box.bottom()), point_t(box.right(), box.top()));
  for (auto &m: grObjs) {
    m.query(bgi::intersects(boostb), back_inserter(temp));
  }
  transform(temp.begin(), temp.end(), back_inserter(result), [](auto &kv) {return kv.second;});
}

void frRegionQuery::queryMarker(const frBox &box, frLayerNum layerNum, vector<frMarker*> &result) {
  vector<rq_rptr_value_t<frMarker> > temp;
  box_t boostb = box_t(point_t(box.left(), box.bottom()), point_t(box.right(), box.top()));
  markers.at(layerNum).query(bgi::intersects(boostb), back_inserter(temp));
  transform(temp.begin(), temp.end(), back_inserter(result), [](auto &kv) {return kv.second;});
}

void frRegionQuery::queryMarker(const frBox &box, vector<frMarker*> &result) {
  vector<rq_rptr_value_t<frMarker> > temp;
  box_t boostb = box_t(point_t(box.left(), box.bottom()), point_t(box.right(), box.top()));
  for (auto &m: markers) {
    m.query(bgi::intersects(boostb), back_inserter(temp));
  }
  transform(temp.begin(), temp.end(), back_inserter(result), [](auto &kv) {return kv.second;});
}

//void frRegionQuery::query(const frBox &box, vector<rq_rptr_value_t<frBlockObject> > &result) {
//  box_t boostb = box_t(point_t(box.left(), box.bottom()), point_t(box.right(), box.top()));
//  for (auto &m: shapes) {
//    m.query(bgi::intersects(boostb), back_inserter(result));
//  }
//  //transform(temp.begin(), temp.end(), back_inserter(result), [](auto &kv) {return kv.second;});
//}



// frRegionQuery 类的成员函数，用于初始化区域查询
void frRegionQuery::init(frLayerNum numLayers) {
  // 清除并重新初始化 shapes、markers 等容器
  shapes.clear();
  shapes.resize(numLayers);

  markers.clear();
  markers.resize(numLayers);
  // 使用 allShapes 容器临时存储所有形状，然后创建 rtree 索引
  vector<vector<rq_rptr_value_t<frBlockObject> > > allShapes(numLayers);

  int cnt = 0;
  for (auto &inst: design->getTopBlock()->getInsts()) {
    for (auto &instTerm: inst->getInstTerms()) {
      add(instTerm.get(), allShapes);
    }
    for (auto &instBlk: inst->getInstBlockages()) {
      add(instBlk.get(), allShapes);
    }
    cnt++;
    // VERBOSE 宏用于控制是否输出调试信息
    if (VERBOSE > 0) {
      if (cnt < 100000) {
        if (cnt % 10000 == 0) {
          cout <<"  complete " <<cnt <<" insts" <<endl;
        }
      } else {
        if (cnt % 100000 == 0) {
          cout <<"  complete " <<cnt <<" insts" <<endl;
        }
      }
    }
  }
  cnt = 0;
  for (auto &term: design->getTopBlock()->getTerms()) {
    add(term.get(), allShapes);
    cnt++;
    if (VERBOSE > 0) {
      if (cnt < 100000) {
        if (cnt % 10000 == 0) {
          cout <<"  complete " <<cnt <<" terms" <<endl;
        }
      } else {
        if (cnt % 100000 == 0) {
          cout <<"  complete " <<cnt <<" terms" <<endl;
        }
      }
    }
  }
  /*
  cnt = 0;
  for (auto &net: design->getTopBlock()->getNets()) {
    for (auto &shape: net->getShapes()) {
      add(shape.get(), allShapes);
    }
    for (auto &via: net->getVias()) {
      add(via.get(), allShapes);
    }
    cnt++;
    if (VERBOSE > 0) {
      if (cnt % 10000 == 0) {
        cout <<"  complete " <<cnt <<" nets" <<endl;
      }
    }
  }
  */
  cnt = 0;
  for (auto &net: design->getTopBlock()->getSNets()) {
    for (auto &shape: net->getShapes()) {
      add(shape.get(), allShapes);
    }
    for (auto &via: net->getVias()) {
      add(via.get(), allShapes);
    }
    cnt++;
    if (VERBOSE > 0) {
      if (cnt % 10000 == 0) {
        cout <<"  complete " <<cnt <<" snets" <<endl;
      }
    }
  }

  cnt = 0;
  for (auto &blk: design->getTopBlock()->getBlockages()) {
    add(blk.get(), allShapes);
    cnt++;
    if (VERBOSE > 0) {
      if (cnt % 10000 == 0) {
        cout <<"  complete " <<cnt <<" blockages" <<endl;
      }
    }
  }

  for (auto i = 0; i < numLayers; i++) {
    shapes.at(i) = boost::move(bgi::rtree<rq_rptr_value_t<frBlockObject>, bgi::quadratic<16> >(allShapes.at(i)));
    allShapes.at(i).clear();
    allShapes.at(i).shrink_to_fit();
    if (VERBOSE > 0) {
      cout <<"  complete " <<design->getTech()->getLayer(i)->getName() <<endl;
    }
  }

}

// frRegionQuery 类的成员函数，用于初始化原始导线（origGuides）的区域查询
//类似于init函数
void frRegionQuery::initOrigGuide(frLayerNum numLayers, map<frNet*, vector<frRect>, frBlockObjectComp> &tmpGuides) {
  origGuides.clear();
  origGuides.resize(numLayers);
  vector<vector<rq_rptr_value_t<frNet> > > allShapes(numLayers);

  int cnt = 0;
  for (auto &[net, rects]: tmpGuides) {
    for (auto &rect: rects) {
      addOrigGuide(net, rect, allShapes);
      cnt++;
      if (VERBOSE > 0) {
        if (cnt < 100000) {
          if (cnt % 10000 == 0) {
            cout <<"  complete " <<cnt <<" orig guides" <<endl;
          }
        } else {
          if (cnt % 100000 == 0) {
            cout <<"  complete " <<cnt <<" orig guides" <<endl;
          }
        }
      }
    }
  }
  for (auto i = 0; i < numLayers; i++) {
    origGuides.at(i) = boost::move(bgi::rtree<rq_rptr_value_t<frNet>, bgi::quadratic<16> >(allShapes.at(i)));
    allShapes.at(i).clear();
    allShapes.at(i).shrink_to_fit();
    if (VERBOSE > 0) {
      cout <<"  complete " <<design->getTech()->getLayer(i)->getName() <<endl;
    }
  }
}

void frRegionQuery::initGuide(frLayerNum numLayers) {
  // guides
  //if (VERBOSE > 0) {
  //  ;
  //}
  guides.clear();
  guides.resize(numLayers);
  vector<vector<rq_rptr_value_t<frGuide> > > allGuides(numLayers);

  int cnt = 0;
  for (auto &net: design->getTopBlock()->getNets()) {
    for (auto &guide: net->getGuides()) {
      addGuide(guide.get(), allGuides);
    }
    cnt++;
    if (VERBOSE > 0) {
      if (cnt < 100000) {
        if (cnt % 10000 == 0) {
          cout <<"  complete " <<cnt <<" nets (guide)" <<endl;
        }
      } else {
        if (cnt % 100000 == 0) {
          cout <<"  complete " <<cnt <<" nets (guide)" <<endl;
        }
      }
    }
  }
  for (auto i = 0; i < numLayers; i++) {
    guides.at(i) = boost::move(bgi::rtree<rq_rptr_value_t<frGuide>, bgi::quadratic<16> >(allGuides.at(i)));
    allGuides.at(i).clear();
    allGuides.at(i).shrink_to_fit();
    if (VERBOSE > 0) {
      cout <<"  complete " <<design->getTech()->getLayer(i)->getName() <<" (guide)"<<endl;
    }
  }
}

// frRegionQuery 类的成员函数，用于初始化全局路由（GR）引脚（pins）的区域查询
void frRegionQuery::initGRPin(vector<pair<frBlockObject*, frPoint> > &in) {
  grPins.clear();
  vector<rq_rptr_value_t<frBlockObject> > allGRPins;
  box_t boostb;
  for (auto &[obj, pt]: in) {
    boostb = box_t(point_t(pt.x(), pt.y()), point_t(pt.x(), pt.y()));
    allGRPins.push_back(make_pair(boostb, obj));
  }
  in.clear();
  in.shrink_to_fit();
  grPins = boost::move(bgi::rtree<rq_rptr_value_t<frBlockObject>, bgi::quadratic<16> >(allGRPins));
}

void frRegionQuery::initRPin(frLayerNum numLayers) {
  rpins.clear();
  rpins.resize(numLayers);

  vector<vector<rq_rptr_value_t<frRPin> > > allRPins(numLayers);

  for (auto &net: design->getTopBlock()->getNets()) {
    for (auto &rpin: net->getRPins()) {
      addRPin(rpin.get(), allRPins);
    }
  }

  for (auto i = 0; i < numLayers; i++) {
    rpins.at(i) = boost::move(bgi::rtree<rq_rptr_value_t<frRPin>, bgi::quadratic<16> >(allRPins.at(i)));
    allRPins.at(i).clear();
    allRPins.at(i).shrink_to_fit();
  }
}

// frRegionQuery 类的成员函数，用于初始化详细路由（DR）对象的区域查询
void frRegionQuery::initDRObj(frLayerNum numLayers) {
  drObjs.clear();
  drObjs.resize(numLayers);

  vector<vector<rq_rptr_value_t<frBlockObject> > > allShapes(numLayers);

  int cnt = 0;
  for (auto &net: design->getTopBlock()->getNets()) {
    for (auto &shape: net->getShapes()) {
      addDRObj(shape.get(), allShapes);
    }
    for (auto &via: net->getVias()) {
      addDRObj(via.get(), allShapes);
    }
    cnt++;
    //if (VERBOSE > 0) {
    //  if (cnt < 100000) {
    //    if (cnt % 10000 == 0) {
    //      cout <<"  complete " <<cnt <<" nets" <<endl;
    //    }
    //  } else {
    //    if (cnt % 100000 == 0) {
    //      cout <<"  complete " <<cnt <<" nets" <<endl;
    //    }
    //  }
    //}
  }

  for (auto i = 0; i < numLayers; i++) {
    drObjs.at(i) = boost::move(bgi::rtree<rq_rptr_value_t<frBlockObject>, bgi::quadratic<16> >(allShapes.at(i)));
    allShapes.at(i).clear();
    allShapes.at(i).shrink_to_fit();
    //if (VERBOSE > 0) {
    //  cout <<"  complete " <<design->getTech()->getLayer(i)->getName() <<endl;
    //}
  }

}

// frRegionQuery 类的成员函数，用于初始化 GR 相关对象的区域查询
void frRegionQuery::initGRObj(frLayerNum numLayers) {
  grObjs.clear();
  grObjs.shrink_to_fit();
  grObjs.resize(numLayers);

  vector<vector<rq_rptr_value_t<grBlockObject> > > allShapes(numLayers);

  for (auto &net: design->getTopBlock()->getNets()) {
    for (auto &shape: net->getGRShapes()) {
      addGRObj(shape.get(), allShapes);
    }
    for (auto &via: net->getGRVias()) {
      addGRObj(via.get(), allShapes);
    }
  }

  for (auto i = 0; i < numLayers; i++) {
    grObjs.at(i) = boost::move(bgi::rtree<rq_rptr_value_t<grBlockObject>, bgi::quadratic<16> >(allShapes.at(i)));
    allShapes.at(i).clear();
    allShapes.at(i).shrink_to_fit();
  }
}

// frRegionQuery 类的成员函数，用于打印 GR 相关对象的区域查询信息
void frRegionQuery::printGRObj() {
  cout <<endl;
  for (int i = 0; i < (int)(design->getTech()->getLayers().size()); i++) {
    frString layerName;
    design->getTech()->getLayers().at(i)->getName(layerName);
    cout <<layerName <<" grObj region query size = " <<grObjs.at(i).size() <<endl <<flush;
  }
}


// frRegionQuery 类的成员函数，用于打印区域查询信息
void frRegionQuery::print() {
  cout <<endl;
  for (int i = 0; i < (int)(design->getTech()->getLayers().size()); i++) {
    frString layerName;
    design->getTech()->getLayers().at(i)->getName(layerName);
    cout <<layerName <<" shape region query size = " <<shapes.at(i).size() <<endl <<flush;
  }
}

// frRegionQuery 类的成员函数，用于打印导线（guides）的区域查询信息
void frRegionQuery::printGuide() {
  cout <<endl;
  for (int i = 0; i < (int)(design->getTech()->getLayers().size()); i++) {
    frString layerName;
    design->getTech()->getLayers().at(i)->getName(layerName);
    cout <<layerName <<" guide region query size = " <<guides.at(i).size() <<endl <<flush;
  }
}

// frRegionQuery 类的成员函数，用于打印 DR 对象的区域查询信息
void frRegionQuery::printDRObj() {
  cout <<endl;
  for (int i = 0; i < (int)(design->getTech()->getLayers().size()); i++) {
    frString layerName;
    design->getTech()->getLayers().at(i)->getName(layerName);
    cout <<layerName <<" drObj region query size = " <<drObjs.at(i).size() <<endl <<flush;
  }
}

// frRegionQuery 类的成员函数，用于清除导线（guides）的区域查询信息
void frRegionQuery::clearGuides() {
  // 遍历 guides 容器，清除所有层级的数据
  for (auto &m: guides) {
    m.clear();
  }
}
