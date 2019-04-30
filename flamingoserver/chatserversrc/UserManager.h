/** 
 *  管理所有的用户信息，初始信息从数据库中加载, UserManager.h
 *  zhangyl 2017.03.15
 **/

#pragma once
#include <stdint.h>
#include <string>
#include <list>
#include <mutex>
#include <set>

using namespace std;

#define GROUPID_BOUBDARY   0x0FFFFFFF 

#define DEFAULT_TEAMNAME  "My Friends"

enum FRIEND_OPERATION
{
    FRIEND_OPERATION_ADD,
    FRIEND_OPERATION_DELETE
};

struct FriendInfo
{
    int32_t friendid;
    string  markname;
    string  teamname;
};

//用户或者群
struct User
{
    int32_t        userid;      //0x0FFFFFFF以上是群号，以下是普通用户
    string         username;    //群账户的username也是群号userid的字符串形式
    string         password;
    string         nickname;    //群账号为群名称
    int32_t        facetype;
    string         customface;
    string         customfacefmt;//自定义头像格式
    int32_t        gender;
    int32_t        birthday;
    string         signature;
    string         address;
    string         phonenumber;
    string         mail;
    /*
    个人用户好友分组信息，对于群账户则为空，例如:
    [{"teamname": "我的好友"}, {"teamname": "我的同事"}, {"teamname": "企业客户"}]
    */
    string             teaminfo;       //对于普通用户，为分组信息；对于群组则为空
    int32_t            ownerid;        //对于群账号，为群主userid
    list<FriendInfo>   friends;        
};

class UserManager final
{
public:
    UserManager();
    ~UserManager();

    bool Init(const char* dbServer, const char* dbUserName, const char* dbPassword, const char* dbName);

    UserManager(const UserManager& rhs) = delete;
    UserManager& operator=(const UserManager& rhs) = delete;

    bool AddUser(User& u);
    bool MakeFriendRelationshipInDB(int32_t smallUserid, int32_t greaterUserid);
    bool ReleaseFriendRelationshipInDBAndMemory(int32_t smallUserid, int32_t greaterUserid);
    bool UpdateUserRelationshipInMemory(int32_t userid, int32_t target, FRIEND_OPERATION operation);
    bool AddFriendToUser(int32_t userid, int32_t friendid);
    bool DeleteFriendToUser(int32_t userid, int32_t friendid);

    bool IsFriend(int32_t userid, int32_t friendid);
    
    //TODO: 这类都是更新用户信息的接口，将来可以考虑统一起来
    bool UpdateUserInfoInDb(int32_t userid, const User& newuserinfo);
    bool ModifyUserPassword(int32_t userid, const std::string& newpassword);
    //将内存和数据库中的某个用户的分组信息改成新的newteaminfo
    bool UpdateUserTeamInfoInDbAndMemory(int32_t userid, const std::string& newteaminfo);
    bool DeleteTeam(int32_t userid, const std::string& deletedteamname);
    bool ModifyTeamName(int32_t userid, const std::string& newteamname, const std::string& oldteamname);
    
    //更新用户好友备注名
    bool UpdateMarknameInDb(int32_t userid, int32_t friendid, const std::string& newmarkname);
    //移动好友至其他分组
    bool MoveFriendToOtherTeam(int32_t userid, int32_t friendid, const std::string& newteamname);

    bool AddGroup(const char* groupname, int32_t ownerid, int32_t& groupid);

#ifdef FXN_VERSION
    bool InsertDeviceInfo(int32_t userid, int32_t deviceid, int32_t classtype, int64_t uploadtime, const std::string& deviceinfo);
#endif

    //聊天消息入库
    bool SaveChatMsgToDb(int32_t senderid, int32_t targetid, const std::string& chatmsg);

    //TODO: 当用户越来越多，并发量越来越多的时候，这个系列的函数效率高吗？
    bool GetUserInfoByUsername(const std::string& username, User& u);
    bool GetUserInfoByUserId(int32_t userid, User& u);
    bool GetUserInfoByUserId(int32_t userid, User*& u);
    bool GetFriendInfoByUserId(int32_t userid, std::list<User>& friends);
    //获取好友的备注名
    bool GetFriendMarknameByUserId(int32_t userid1, int32_t friendid, std::string& markname);
    bool GetTeamInfoByUserId(int32_t userid, std::string& teaminfo);

private:
    bool LoadUsersFromDb();
    bool LoadRelationshipFromDb(int32_t userid, std::list<FriendInfo>& r);

private:
    int                 m_baseUserId{ 0 };        //m_baseUserId, 取数据库里面userid最大值，新增用户在这个基础上递增
    int                 m_baseGroupId{0x0FFFFFFF};
    list<User>          m_allCachedUsers;
    mutex               m_mutex;

    string              m_strDbServer;
    string              m_strDbUserName;
    string              m_strDbPassword;
    string              m_strDbName;
};