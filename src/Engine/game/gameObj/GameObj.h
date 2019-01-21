/*
 * ========================= GameObj.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.24
 *                                        修改 -- 2018.11.24
 * ----------------------------------------------------------
 *    GameObj 是游戏中的 一等公民。
 *    可以作为一个 独立的单位，存在于 游戏中
 * ----------------------------
 */
#ifndef _TPR_GAME_OBJ_H_
#define _TPR_GAME_OBJ_H_

//--- glm - 0.9.8 ---
#include <glm/glm.hpp>
            //-- glm::vec2
            //-- glm::vec3
            //-- glm::vec4
            //-- glm::mat4
#include <glm/gtc/matrix_transform.hpp>
            //-- glm::translate
            //-- glm::rotate
            //-- glm::scale
            //-- glm::perspective
#include <glm/gtc/type_ptr.hpp> 
            //-- glm::value_ptr

//-------------------- CPP --------------------//
#include <string>
#include <vector>
#include <functional>

//------------------- Libs --------------------//
#include "tprDataType.h" 

//-------------------- Engine --------------------//
#include "GameObjType.h" 
#include "GameMesh.h" 
#include "ID_Manager.h" 
#include "IntVec.h" 
#include "Move.h"
#include "MapCoord.h" 
#include "GameObjPos.h"



//--- 最基础的 go 类，就像一个 "伪接口" ----//
// 具象go类 并不用继承 基础go类，而是 “装配” 一个go实例，
// 通过  动态绑定 来配置 这个被绑定的 go实例的数据。
// 通过 function / bind 动态绑定各种回调函数
// 在 主引擎中，go类 是官方认可的通用类型。也只能通过这个 类来 访问 一切 go实例
//-------
//  并不存在孤立的 go实例，每个go实例，都被一个 具象go实例 所"表达“
//  goid 是全局唯一的。 其外层的 具象go实例 也使用这个 id号
//-------
//  go类 与 具象go类：
//  -- go类实例 负责存储实际的数据
//  -- 具象go类 只是一个 “装配工厂”，不存在 较长生命周期的 “具象go类实例”
class GameObj{
    using F_GO  = std::function<void(GameObj*)>;
public:
    GameObj() = default;

    void init();//-- MUST --

    //-- disl <-> mem --//
    void        d2m( diskGameObj *_dgo );
    diskGameObj m2d();

    //---------------- callback -----------------//
    F_GO  Awake {nullptr};  //- unused
    F_GO  Start {nullptr};  //- unused

    F_GO  RenderUpdate {nullptr}; //- 每1渲染帧，被引擎调用
    F_GO  LogicUpdate  {nullptr}; //- 每1逻辑帧，被主程序调用 （帧周期未定）
    F_GO  BeAffect     {nullptr}; //- 当 本go实例 被外部 施加技能／影响 时，调用的函数
                                  //- 未来会添加一个 参数：“被施加技能的类型”

    //----------------- self vals ---------------//
    goid_t         id       {NULLID};    
    goSpecId_t     species  {0};                //- go species id
    GameObjFamily  family   {GameObjFamily::Major};  

    bool   isTopGo   {true}; //- 是否为 顶层 go (有些go只是 其他go 的一部分)
    goid_t parentId {NULLID}; //- 不管是否为顶层go，都可以有自己的 父go。
                             //- 如果没有，此项写 NULLID

    //---- go 状态 ----//
    bool              isActive {false}; //- 是否进入激活圈. 
                                        //   未进入激活圈的go，不参与任何逻辑运算，也不会被渲染
    GameObjState      state     {GameObjState::Sleep};         //- 常规状态
    GameObjMoveState  moveState {GameObjMoveState::BeMovable}; //- 运动状态
    
    //--- move sys ---//
    GameObjPos   goPos {}; 
    Move         move  {};


    float  weight {0}; //- go重量 （影响自己是否会被 一个 force 推动）

    bool isDirty {false};  //- 是否为 默认go（是否被改写过）
                            //- “默认go” 意味着这个 go没有被游戏改写过。
                            //- 当它跟着 mapSection 存入硬盘时，会被转换为 go_species 信息。
                            //- 以便少存储 一份 go实例，节省 硬盘空间。

    bool isControlByPlayer  {false}; 


    //-- 也许该用 umap 来管理，尤其是 gameMesh实例很多时。
    // - rootGameMesh  -- 排在第一个，或者name==“root” 的 gameMesh实例
    // - childGameMesh -- 剩下的其余实例
    std::vector<GameMesh> gameMeshs {}; //- go实例 与 GameMesh实例 强关联
                            // 大部分go不会卸载／增加自己的 GameMesh实例
                            //- 在一个 具象go类实例 的创建过程中，会把特定的 GameMesh实例 存入此容器
                            //- 只存储在 mem态。 在go实例存入 硬盘时，GameMesh实例会被丢弃
                            //- 等再次从section 加载时，再根据 具象go类型，生成新的 GameMesh实例。

                // *** 此容器 疑似引发了一个 史诗级BUG... ***
                // 当在 具象类init() 中，调用 gameMeshs.size() 等之类的语句时，程序就正常。
                // 但如果不调用，程序无法显示图形
                // 目前还没搞清原因
                // --- 这个 bug 暂时消失了... ---

    Collision    collision {}; //- 一个go实例，对应一个 collision实例。强关联


    //----------- binary chunk -------------//         
    std::vector<u8>  binary; //- 具象go类 定义的 二进制数据块。真实存储地
                            //- binary 本质是一个 C struct。 由 具象go类方法 使用。
                            //- binary 可能会跟随 go实例 存储到 硬盘态。（未定...）

    //----------- funcs --------------//
    //void debug(); //- 打印 本go实例 的所有信息
    GameMesh *creat_new_gameMesh(); 


    //-- 代表整个go实例 的 rootAnchorPos --
    //  放得非常深，通过多层调用才实现...
    inline const AnchorPos &get_rootAnchorPos() const {
        return gameMeshs.at(0).get_rootAnchorPos();
    }


    //------------ static ----------//
    static ID_Manager  id_manager; //- 负责生产 go_id ( 在.cpp文件中初始化 )

};




#endif

