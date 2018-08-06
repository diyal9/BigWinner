//
//  CRequestMsgConstDedine.h

#ifndef  CRequestMsgConstDedine_h
#define  CRequestMsgConstDedine_h



#define GAME_URL                "117.135.139.104/egame//landlord/api.htm?" 

#define IMAGEDOWN           "http://qllordazc.emmasp.com/egame//landlord/api.htm?"


#define SOCKET_PACKET                   65536
#define SOCKET_BUFFER                   4896
#define SOCKET_ERROR                    -1



#define CMD_ERROR           9999
#define CMD_LOGIN              1      // 登陆(C->G)
#define CMD_LOGIN_REPLY        2      // 登陆回复(G->C)
#define CMD_MYINFO             3      // 查询个人信息(C->G)
#define CMD_MYINFO_REPLY       4      // 查询个人信息回复(G->C)
#define CMD_STORES             5      // 查询商铺（C->G）
#define CMD_STORES_REPLY       6      // 查询商铺回复（G->C）
#define CMD_SHELFS             7      // 查询货架列表（C->G）
#define CMD_SHELFS_REPLY       8      // 查询货架列表回复(G->C)
#define CMD_JOIN               9      // 进入货架(C->G)
#define CMD_JOIN_REPLY        10      // 进入货架回复(G->C)
#define CMD_ANSWER            11      // 答题(C->G)
#define CMD_QUESTION          12      // 题目发送（G->C）
#define CMD_PICKUP            13      // 捡到印花（C->G）
#define CMD_EXCHANGE          14      // 申请换题（C->G）


#define CMD_PVEROUNDEND       20      // 货架答题结束

#define CMD_USEPROP           23      // 闯关使用道具
#define CMD_USEPROP_REPLY     24      // 闯关使用道具回复

#define CMD_REMOVETIME        21      // 请求消除等待缓冲时间
#define CMD_REMOVETIME_REPLY  22      // 返回消除缓冲时间结果

#define CMD_DIRECTPASS        27      // 请求直接送礼过关
#define CMD_DIRECTPASS_REPLY  28      // 返回直接送礼过关


#define CMD_PK1_ROOMS         31      // C->S: 请求所有对战房间
#define CMD_PK1_ROOMS_REPLY   32      // S->C: 返回所有对战房间
#define CMD_PK1_JOIN          33      // C->S: 进入对战房间并等待匹配
#define CMD_PK1_JOIN_REPLY    34      // S->C: 返回对战房间
#define CMD_PK1_QUESTION      35      // S->C: 服务器通知下一题目
#define CMD_PK1_ANSWER        36      // C->S: 回答题目
#define CMD_PK1_USEPROP       37      // C->S: 对战中使用道具, 使用后同时通知双方
#define CMD_PK1_USEPROP_REPLY 38      // S->C: 使用道具后回复
#define CMD_PK1_EQUIP         39      // C->S: 装备
#define CMD_PK1_EQUIP_REPLY   40      // S->C: 返回信息
#define CMD_PK1_USECARD       41      // C->S: 对战中使用题卡
#define CMD_PK1_USECARD_REPLY 42      // S->C: 使用题卡回复
#define CMD_PK1_EXIT          44      // C->S: 退出等待
#define CMD_PK1_EXIT_REPLY    45      // S->C: 返回退出等待信息
#define CMD_PK1_PAIR          46      // S->C: 通知匹配成功的信息
#define CMD_PK1_GETEQUIP      47      // C->S: 获取当前装备信息


//wangguolong
#define CMD_PREPAREFIGHT         48  // C->S: 获取能用于对战的道具,角色,题卡信息
#define CMD_PK1_BACKPACK_REPLY   49 // S->C: 备战设置,返回个人背包里可以用来对战的道具,角色,题卡信息

#define CMD_PK1_RANKINGLIST         50  // C->S: 获取对战排行榜
#define CMD_PK1_RANKINGLIST_REPLY   51 // S->C: 返回对战排行榜

#define CMD_PK1_PASS          55      // S->C: 对战结束通知
#define CMD_PK1_PROP_START    56      // S->C: 通知客户端开始使用题卡

#define CMD_UPPLYINFO        57      // C->S: 请求修改玩家信息
#define CMD_UPPLYINFO_REPLY  58      // S->C: 返回修改成功玩家信息

//wangguolong 20131104
#define CMD_PRIZESHOW        81      // C->S: 查询奖品橱窗功能
#define CMD_PRIZESHOW_REPLY  82      // S->C: 返回奖品橱窗的信息
//wangguolong 20131107
#define CMD_PRIZEEXCHANGEOK         85         // C->S: 在兑换页点击确认兑换，并且在确认框点击【确认】后
#define CMD_PRIZEEXCHANGEOK_REPLY   86         // S->C: 返回兑换成功信息

#define CMD_PRIZEINFO         87         // C->S: 在奖品橱窗界面点击，查看信息，查看印花商品的情况
#define CMD_PRIZEINFO_REPLY   88         // S->C: 返回印花商品的情况

//兑换相关 add by yin
#define CMD_SENDPHONE            89       // C->S: 发送要兑换的手机号
#define CMD_SENDPHONE_REPLY      891      // S->C: 返回确认码（注：临时使用）
#define CMD_SENDCHECKCODE        90       // C->S: 发送要兑换手机号的验证码
#define CMD_SENDCHECKCODE_REPLY  91       // S->C: 返回验证成功信息

#define CMD_PLYACHIEVE           92       // C->S: 查看成就信息
#define CMD_PLYACHIEVE_REPLY     93       // S->C: 返回成就信息
#define CMD_ACHIEVE_SEND         94       // S->C: 主动发送成就领取信息（闯关或对战结束后，玩家达到成就要求则发送）
#define CMD_GETACHIEVE           95       // C->S: 领取成就信息
#define CMD_GETACHIEVE_REPLY     96       // S->C: 返回取成功信息
#define CMD_SHOWACHIEVE_REPLY    98       // S->C: 返回玩家最新的成就信息


#define CMD_BACKPACK         101   // C->S: 查询个人背包
#define CMD_BACKPACK_REPLY   102   // S->C: 返回个人背包的信息

#define CMD_CARDCOMPOSE         103   // C->S: 需要合并卡牌信息
#define CMD_CARDCOMPOSE_REPLY   104   // S->C: 返回合并成功后的个人卡牌信息

#define CMD_SHOPSHOW         105   // C->S: 查询商店信息
#define CMD_SHOPSHOW_REPLY   106   // S->C: 返回商店信息
#define CMD_SHOPBUY          107   // S->C: C->S: 确认购买(非RMB购买)
#define CMD_SHOPBUY_REPLY    108   // S->C: 返回购买成功信息(非RMB购买)
#define CMD_RMBBUYCARD       109      // C->S: 请求RMB购买购物卡信息
#define CMD_RMBBUYCARD_REPLY 110      // S->C: 返回RMB购买购物卡信息
#define CMD_PK1_SUBMITED     114      // S->C: 通知出题方，答题方选择的选项


#define ERR_SUCCEED            0 // 正确 / 正常 / 成功
#define ERR_FAILED          9999 // 打开失败 / 操作失败 / 连接失败
#define ERR_TIMEOUT         9998 // 操作超时
#define ERR_NO_INPUT        9997 // 缺少必要的输入 / 必输项遗漏
#define ERR_NO_PARAMS       9996 // 缺少参数
#define ERR_PARAMS          9995 // 参数内容错误
#define ERR_NOT_EXISTS      9994 // 不存在
#define ERR_EXISTS          9993 // 已经存在
#define ERR_BUSY            9992 // 忙 / 操作被忽略
#define ERR_LOAD            9991 // 读取失败 / 装载失败
#define ERR_SAVE            9990 // 写入失败 / 保存失败
#define ERR_CONTENT         9989 // 内容错误 / 内容无法识别
#define ERR_NO_PERMISSIONS  9988 // 权限不足
#define ERR_SYSTEM          9987 // 系统错误
#define ERR_LOCKED          9986 // 已经锁定
#define ERR_NOT_ACTIVE      9985 // 尚未激活
#define ERR_NOT_LOGGED_IN   9984 // 尚未登录
#define ERR_NOT_SCENE       9983 // 场景错误

/*
 http 注册ID
 */
enum CHTTP_REQUEST_ID
{
	CONTINUATIONLOGON = 10,
	

};


#endif