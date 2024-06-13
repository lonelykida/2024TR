//定义了项目中使用的基本类型和枚举，为整个项目提供了类型安全和一致性
//定义了项目中使用的基本类型、枚举、模板别名以及与几何、图形和电路设计相关的数据结构

#ifndef _FR_BASE_TYPES_H_
#define _FR_BASE_TYPES_H_

#include <vector>
#include <list>
#include <map>
#include <string>
#include <utility>

//#include <boost/property_map/property_map.hpp>

//adjacency_list 是BOOST库中实现的一种图形表示方法，使用邻接表来存储图形的边和顶点信息
#include <boost/graph/adjacency_list.hpp>
//#include <boost/graph/connected_components.hpp>

// 包含 Boost.Geometry 库，用于处理几何对象和运算
#include <boost/geometry.hpp>
#include <boost/polygon/polygon.hpp>
//#include <boost/geometry/geometries/point.hpp>
//#include <boost/geometry/geometries/box.hpp>
//#include <boost/geometry/index/rtree.hpp>

namespace fr {
  //int
  using frLayerNum = int;
  //int
  using frCoord = int;
  //unsigned int
  using frUInt4 = unsigned int;
  //double
  using frDist  = double;
  //string
  using frString = std::string;
  //unsigned int
  using frCost = unsigned int;
  //int - 期望值为负
  using frMIdx = int; // negative value expected - 期望值为负
  template <typename T>
  using frCollection = std::vector<T>;  //vector<T>
  template <typename T>
  using frVector     = std::vector<T>;  //vector<T>
  template <typename T>
  using frList       = std::list<T>;  //list<T>
  template <typename T>
  using frListIter   = typename std::list<T>::iterator; //迭代器
  //template <typename T>
  //bool frListIterComp (const frListIter<T>& a, const frListIter<T>& b) {
  //  return &(*a) < &(*b);
  //};

  // 定义一组枚举类型，表示不同的 frOrientEnum 方向
  enum frOrientEnum {
      frcR0       = 0, // N 北
      frcR90      = 1, // W 西
      frcR180     = 2, // S 南
      frcR270     = 3, // E 东
      frcMY       = 4, // FN 镜像翻转北
      frcMXR90    = 5, // FW 镜像翻转西
      frcMX       = 6, // FS 镜像翻转南
      frcMYR90    = 7  // FE 镜像翻转东
  };

  // 定义一组枚举类型，表示不同的 frEndStyleEnum 端点样式
  enum frEndStyleEnum {
      frcTruncateEndStyle = 0, // ext = 0 截断端点样式
      frcExtendEndStyle   = 1, // ext = half width 扩展端点样式
      frcVariableEndStyle = 2  // ext = variable 可变端点样式
  };
  // 定义一组枚举类型，表示不同的 frPrefRoutingDirEnum 优选布线方向
  enum frPrefRoutingDirEnum {
      frcNotApplicablePrefRoutingDir = 0,   //不适用
      frcNonePrefRoutingDir          = 1,   //无
      frcHorzPrefRoutingDir          = 2,   //水平
      frcVertPrefRoutingDir          = 3    //垂直
  };
  // 定义一组枚举类型，表示不同的 frBlockObjectEnum 块对象类型
  enum frBlockObjectEnum {
      frcBlockObject   = -1,
      frcNet           = 0,
      frcTerm          = 1,
      frcInst          = 2,
      frcFig           = 3,
      frcConnFig       = 4,
      frcPinFig        = 5,
      frcShape         = 6,
      frcRef           = 7,
      frcVia           = 8,
      frcPin           = 9,
      frcInstTerm      = 10,
      frcRect          = 11,
      frcPolygon       = 12,
      frcSteiner       = 13,
      frcRoute         = 14,
      frcPathSeg       = 15,
      frcGuide         = 16,
      frcBlockage      = 17,
      frcLayerBlockage = 18,
      frcBlock         = 19,
      frcBoundary      = 20,
      frcFlexPathSeg   = 21,
      frcFlexVia       = 22,
      frcInstBlockage  = 23,
      frcAccessPattern = 24,
      frcMarker        = 25,
      frcPatchWire     = 26,  //下一个是27，逐渐往后排
      frcRPin,
      grcBlockObject,
      grcFig,
      grcConnFig,
      grcPinFig,
      grcShape,
      grcNet,
      grcPin,
      grcAccessPattern,
      grcPathSeg,
      grcRef,
      grcVia,
      drcBlockObject,
      drcNet,
      drcPin,
      drcAccessPattern,
      drcPathSeg,
      drcVia,
      drcMazeMarker,
      drcFig,
      drcConnFig,
      drcRef,
      drcPinFig,
      drcPatchWire,
      tacBlockObject,
      tacTrack,
      tacPin,
      tacPathSeg,
      tacVia,
      gccBlockObject,
      gccNet,
      gccPin,
      gccEdge,
      gccRect,
      gccPolygon,
      frcAccessPoint,
      frcAccessPoints,
      frcNode,
      grcNode
  };
  //enum class drBlockObjectEnum {
  //  drcBlockObject = 0,
  //  drcNet,
  //  drcPin,
  //  drcAccessPattern,
  //  drcPathSeg,
  //  drcVia
  //};

  // 定义一组枚举类型，表示不同的 frGuideEnum 布线引导类型
  enum class frGuideEnum {
      frcGuideX,
      frcGuideGlobal,
      frcGuideTrunk,
      frcGuideShortConn
  };
  // 定义一组枚举类型，表示不同的 frTermEnum 终端类型
  enum class frTermEnum {
    frcNormalTerm,
    frcClockTerm,
    frcPowerTerm,
    frcGroundTerm
  };
  // 定义一组枚举类型，表示不同的 frNetEnum 网络类型
  enum class frNetEnum {
    frcNormalNet,
    frcClockNet,
    frcPowerNet,
    frcGroundNet
  };
  // 定义一组枚举类型，表示不同的 frTermDirectionEnum 终端方向
  enum class frTermDirectionEnum {
    UNKNOWN,
    INPUT,
    OUTPUT,
    INOUT,
    FEEDTHRU,
  };
  // 定义一组枚举类型，表示不同的 frNodeTypeEnum 节点类型
  enum class frNodeTypeEnum {
    frcSteiner,     
    frcBoundaryPin, 
    frcPin 
  };
  //enum class frLef58CutSpacingTableTypeEnum {
  //  frcCenterSpacing,
  //  frcOrthogonal,
  //  frcOther,
  //};

  // 定义一组枚举类型，表示不同的 frConstraintTypeEnum 约束类型
  enum class frConstraintTypeEnum { // check FlexDR.h fixMode
    frcShortConstraint = 0,   //短路约束
    frcAreaConstraint = 1,    //面积约束
    frcMinWidthConstraint = 2,//最小宽度约束
    frcSpacingConstraint = 3, //间距约束
    frcSpacingEndOfLineConstraint = 4,  //EOL约束
    frcSpacingEndOfLineParallelEdgeConstraint = 5, // not supported
    frcSpacingTableConstraint = 6, // not supported
    frcSpacingTablePrlConstraint = 7, //PRL间距表约束
    frcSpacingTableTwConstraint = 8,
    frcLef58SpacingTableConstraint = 9, // not supported
    frcLef58CutSpacingTableConstraint = 10, // not supported
    frcLef58CutSpacingTablePrlConstraint = 11, // not supported
    frcLef58CutSpacingTableLayerConstraint = 12, // not supported
    frcLef58CutSpacingConstraint = 13, // not supported
    frcLef58CutSpacingParallelWithinConstraint = 14, // not supported
    frcLef58CutSpacingAdjacentCutsConstraint = 15, // not supported
    frcLef58CutSpacingLayerConstraint = 16, // not supported
    frcCutSpacingConstraint = 17,
    frcMinStepConstraint,
    frcLef58MinStepConstraint,
    frcMinimumcutConstraint,
    frcOffGridConstraint,
    frcMinEnclosedAreaConstraint,
    frcLef58CornerSpacingConstraint, // not supported
    frcLef58CornerSpacingConcaveCornerConstraint, // not supported
    frcLef58CornerSpacingConvexCornerConstraint, // not supported
    frcLef58CornerSpacingSpacingConstraint, // not supported
    frcLef58CornerSpacingSpacing1DConstraint, // not supported
    frcLef58CornerSpacingSpacing2DConstraint, // not supported
    frcLef58SpacingEndOfLineConstraint, // not supported
    frcLef58SpacingEndOfLineWithinConstraint, // not supported
    frcLef58SpacingEndOfLineWithinEndToEndConstraint, // not supported
    frcLef58SpacingEndOfLineWithinParallelEdgeConstraint, // not supported
    frcLef58SpacingEndOfLineWithinMaxMinLengthConstraint, // not supported
    frcLef58CutClassConstraint, // not supported
    frcNonSufficientMetalConstraint,
    frcSpacingSamenetConstraint,
    frcLef58RightWayOnGridOnlyConstraint,
    frcLef58RectOnlyConstraint,
    frcRecheckConstraint
  };

  // 定义一组枚举类型，表示不同的 frCornerTypeEnum 角点类型
  enum class frCornerTypeEnum {
    UNKNOWN,
    CONCAVE,  //凹
    CONVEX    //凸
  };

  // 定义一组枚举类型，表示不同的 frCornerDirEnum 角点方向
  enum class frCornerDirEnum {
    UNKNOWN,
    NE,
    SE,
    SW,
    NW
  };

  // 定义一组枚举类型，表示不同的 frMinimumcutConnectionEnum 最小割连接类型
  enum class frMinimumcutConnectionEnum {
    UNKNOWN = -1,
    FROMABOVE = 0,  //从上
    FROMBELOW = 1   //从下
  };

  // 定义一组枚举类型，表示不同的 frMinstepTypeEnum 最小步进类型
  enum class frMinstepTypeEnum {
    UNKNOWN = -1,
    INSIDECORNER = 0,
    OUTSIDECORNER = 1,
    STEP = 2
  };

  //enum class frDirEnum { UNKNOWN = 0, E = 1, S = 2, W = 3, N = 4, U = 5, D = 6 };
  //enum class frDirEnum { UNKNOWN = 0, E = 4, S = 2, W = 3, N = 1, U = 6, D = 5 };
  #define OPPOSITEDIR 7 // used in FlexGC_main.cpp
  // 定义一组枚举类型，表示不同的 frDirEnum 方向
  enum class frDirEnum { UNKNOWN = 0, D = 1, S = 2, W = 3, E = 4, N = 5, U = 6 };

  // 定义一组枚举类型，表示不同的 frLayerTypeEnum 层类型
  enum class frLayerTypeEnum {
    CUT,
    ROUTING,
    IMPLANT,
    MASTERSLICE
  };

  // 定义一组枚举类型，表示不同的 AccessPointTypeEnum 访问点类型
  enum class AccessPointTypeEnum {
    Ideal,
    Good,
    Offgrid,
    None
  };

  // 定义一组枚举类型，表示不同的 MacroClassEnum 宏单元类别
  enum class MacroClassEnum {
    UNKNOWN,
    CORE,
    CORE_TIEHIGH,
    CORE_TIELOW,
    CORE_WELLTAP,
    CORE_SPACER,
    CORE_ANTENNACELL,
    COVER, // GF14
    ENDCAP_PRE,
    BLOCK,
    RING, // ispd19
    PAD, // ispd19
    PAD_POWER, // ispd19
    PAD_SPACER, // GF14
    ENDCAP_BOTTOMLEFT // ispd19
  };

  // note: FlexPA hardcoded the cost, don't change here
  // 定义一组枚举类型，表示不同的 frAccessPointEnum 访问点枚举
  enum class frAccessPointEnum {
    frcOnGridAP   = 0,  //网格上接点
    frcHalfGridAP = 1,  //半网格上接点
    frcCenterAP   = 2,  //中心接点
    frcEncOptAP   = 3   //其他接点
  };

  //enum class drNetOrderingEnum {
  //  NETDRIVEN,
  //  MARKERDRIVEN
  //};

  //enum class gcPinTypeEnum {
  //  FIXED,
  //  ROUTE, 
  //  MERGED
  //};

  //enum frShapeEnum {
  //    frcRect    = 0,
  //    frcPolygon = 1
  //};
  class frBlockObject;
  // 下边定义一些结构体和类型别名，用于 Boost.Graph 图形库
  struct vertex_properties_t {
    frBlockObject* objPtr;
    //int index;
    //boost::default_color_type color;
    //frString name;
  };
  //class frRoute;
  class frConnFig;
  class frInstTerm;
  class frTerm;
  class frInst;
  class frBlockage;
  struct edge_properties_t {
    //std::shared_ptr<frBlockObject> objPtr;
    //std::shared_ptr<frRoute> objPtr;
    std::shared_ptr<frConnFig> objPtr;
    //frString name;
  };
  // 使用 Boost.Graph 图形库定义图类型
  // boost graph
  typedef boost::adjacency_list< boost::listS, boost::listS, boost::undirectedS, vertex_properties_t, edge_properties_t > graph_t;
  //typedef boost::adjacency_list< boost::vecS, boost::vecS, boost::undirectedS, vertex_properties_t, edge_properties_t > graph_t;
  // descriptor
  // 使用 Boost.Graph 图形库定义各种迭代器类型
  typedef boost::graph_traits<graph_t>::vertex_descriptor   vertex_descriptor_t;
  typedef boost::graph_traits<graph_t>::edge_descriptor     edge_descriptor_t;
  // iterator
  typedef boost::graph_traits<graph_t>::vertex_iterator     vertex_iterator_t;
  typedef boost::graph_traits<graph_t>::edge_iterator       edge_iterator_t;
  typedef boost::graph_traits<graph_t>::out_edge_iterator   out_edge_iterator_t;
  typedef boost::graph_traits<graph_t>::adjacency_iterator  adjacency_iterator_t;

  // 定义一个类型别名 vertex_descriptor_map_t，用于映射顶点描述符
  typedef std::map<vertex_descriptor_t, std::size_t>        vertex_descriptor_map_t;
  //typedef boost::property_map<graph_t, &vertex_properties_t::objPtr>::type           tempPM;
  // 使用 Boost.Geometry 命名空间中的 bg 和 bgi
  namespace bg  = boost::geometry;
  namespace bgi = boost::geometry::index;

  //typedef bg::model::point<int, 2, bg::cs::cartesian> boostPoint;
  // 定义一些 Boost.Geometry 中使用的类型别名
  typedef bg::model::d2::point_xy<frCoord, bg::cs::cartesian> boostPoint;
  typedef bg::model::box<boostPoint> boostBox;
  typedef bg::model::polygon<boostPoint> boostPolygon;
  typedef bg::model::segment<boostPoint> boostSegment;



  //typedef boost::geometry::model::point<frCoord, 2, boost::geometry::cs::cartesian>   point_t;
  typedef bg::model::d2::point_xy<frCoord, bg::cs::cartesian>                         point_t;
  typedef bg::model::box<point_t>                                                     box_t;
  typedef bg::model::segment<point_t>                                                 segment_t;
  class frConnFig;
  typedef std::pair<box_t, std::shared_ptr<frConnFig> >                               rtree_frConnFig_value_t;
  //typedef std::pair<box_t, int* >                                                     rtree_test_t;
  typedef std::pair<box_t, std::shared_ptr<frInst> > rtree_frInst_value_t;
  typedef std::pair<box_t, std::shared_ptr<frTerm> > rtree_frTerm_value_t;
  typedef std::pair<box_t, std::pair<std::shared_ptr<frTerm>, std::shared_ptr<frInstTerm> > > rtree_frTerm_frInstTerm_value_t;
  typedef std::pair<box_t, std::shared_ptr<frBlockage> > rtree_frBlockage_value_t;
  //typedef std::pair<box_t, std::string >                                              rtree_value_t;
  template <typename T>
  using rq_iter_value_t = std::pair<box_t, frListIter<T> >;
  template <typename T>
  using rq_ptr_value_t  = std::pair<box_t, std::shared_ptr<T> >;
  template <typename T>
  using rq_rptr_value_t = std::pair<box_t, T* >;
  template <typename T>
  using rq_generic_value_t = std::pair<box_t, T>;

  // KMB data types
  // 定义 KMBGraph 类型，用于 KMB 算法的图表示
  typedef boost::adjacency_list < boost::vecS, boost::vecS, boost::undirectedS, vertex_properties_t, boost::property < boost::edge_weight_t, double > > KMBGraph; 
  // typedef std::pair<int, int> KMBEdge;

  // 定义一些用于DRC检查相关的类型
  // DRC check types
  typedef std::pair<boostPoint, boostPoint> boostEdge;


  // BoostPolygon
  // 定义 Boost.Polygon 相关的类型别名
  typedef boost::polygon::rectangle_data<int>  Rectangle;
  typedef boost::polygon::polygon_90_data<int> Polygon;
  typedef std::vector<boost::polygon::polygon_90_data<int> > PolygonSet;
  typedef boost::polygon::point_data<int> Point;
  typedef boost::polygon::interval_data<int> Interval;
  typedef boost::polygon::segment_data<int> Segment;
}

#endif
