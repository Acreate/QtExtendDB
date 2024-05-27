#ifndef SQLITEDEPOSITORY_H_H_HEAD__FILE__
#define SQLITEDEPOSITORY_H_H_HEAD__FILE__
#pragma once

#include "../../../nameSpace/cylDB.h"
#include "../../dbInterface/I_DB.h"
#include "../../dbInterface/i_depositoryInterface/I_Depository.h"
class QSqlDatabase;
namespace cylDB {
	class QTEXTENDDB_EXPORT SQliteDepository : public I_Depository {
	private:
		std::shared_ptr< QSqlDatabase > database; // 数据库链接对象
		QString linkPath; // 链接路径
		QString user; // 用户名称
		QString password; // 用户密码
	public:
		SQliteDepository( const QString &link_path, const QString &user, const QString &password );
		SQliteDepository( const QString &link_path );
		~SQliteDepository( ) override;
	private: // 私有成员
		SQliteDepository( const std::shared_ptr< QSqlDatabase > &database, const QString &db_path );
	public: // - 接口实现

		/// <summary>
		/// 获取数据库名称
		/// </summary>
		/// <returns>名称</returns>
		QString getDBName( ) const override;
		/// <summary>
		/// 是否存在表
		/// </summary>
		/// <param name="tab_name"></param>
		/// <returns></returns>
		bool hasTab( const QString &tab_name ) const override;	/// <summary>
		/// 创建表
		/// </summary>
		/// <param name="tab_name">表名称</param>
		/// <param name="tab_info">表信息</param>
		/// <param name="key">关键 key</param>
		/// <param name="key_type">key 的类型</param>
		/// <returns>成功返回 true</returns>
		bool createTab( const QString &tab_name, const QVariantMap &tab_info, const QString &key, const QString &key_type ) const override;
		/// <summary>
		/// 创建表
		/// </summary>
		/// <param name="tab_name">表名称</param>
		/// <param name="tab_info">表信息</param>
		/// <returns>成功返回 true</returns>
		bool createTab( const QString &tab_name, const QVariantMap &tab_info ) const override;
		/// <summary>
		/// 获取表信息
		/// </summary>
		/// <param name="tab_name">表明</param>
		/// <returns>返回所有列的信息</returns>
		IResultInfo_Shared getTabInfo( const QString &tab_name ) const override;
		/// <summary>
		/// 在表中创建一项
		/// </summary>
		/// <param name="tab_name">表名称</param>
		/// <param name="item_name">项名</param>
		/// <returns>成功返回 true</returns>
		bool addItem( const QString &tab_name, const QString &item_name ) const override;
		/// <summary>
		/// 删除表中的一项
		/// </summary>
		/// <param name="tab_name">表名称</param>
		/// <param name="item_name">项名</param>
		/// <returns>成功返回 true</returns>
		bool removeItem( const QString &tab_name, const QString &item_name ) const override;
		/// <summary>
		/// 在表当中插入数据
		/// </summary>
		/// <param name="tab_name">表名称</param>
		/// <param name="variants">数据列表</param>
		/// <returns>成功返回 true</returns>
		bool appendData( const QString &tab_name, QVector< QVariant > variants ) const override;
		/// <summary>
		/// 在表当中更新数据
		/// </summary>
		/// <param name="tab_name">表名称</param>
		/// <param name="var_map_s">数据列表</param>
		/// <returns>成功返回 true</returns>
		bool updateData( const QString &tab_name, QMap< QVariant, QVariant > var_map_s ) const override;
		/// <summary>
		/// 删除表中信息，满足条件为完整匹配 var_map_s 参数
		/// </summary>
		/// <param name="tab_name">表名称</param>
		/// <param name="var_map_s">数据列表</param>
		/// <returns>成功返回 true</returns>
		bool removeData( const QString &tab_name, QMap< QVariant, QVariant > var_map_s ) const override;
		/// <summary>
		/// 设置用户信息
		/// </summary>
		/// <param name="user">用户名称</param>
		/// <param name="password">用户密码</param>
		void setUserInfo( const QString &user, const QString &password ) override;
		/// <summary>
		/// 打开数据库
		/// </summary>
		/// <returns>成功返回 true</returns>
		bool isOpen( ) const override;
		/// <summary>
		/// 打开数据库
		/// </summary>
		/// <returns>成功返回 true</returns>
		bool open( ) const override;
		/// <summary>
		/// 打开数据库
		/// </summary>
		/// <param name="user">用户名</param>
		/// <param name="password">用户密码</param>
		/// <returns>成功返回 true</returns>
		bool open( const QString &user, const QString &password ) override;
		/// <summary>
		/// 关闭数据库
		/// </summary>
		/// <returns>成功返回 true</returns>
		bool close( ) const override;
		/// <summary>
		/// 提交
		/// </summary>
		/// <returns>成功返回 true</returns>
		bool commit( ) const override;
		/// <summary>
		/// 是否打开错误
		/// </summary>
		/// <returns>打开错误返回 true</returns>
		bool isOpenError( ) const override;
		/// <summary>
		/// 获取最后的错误
		/// </summary>
		/// <returns>错误信息</returns>
		QSqlError getLastError( ) const override;
	public:		// - 静态功能
		/// <summary>
		/// 使用路径创建一个 sqlite 数据库
		/// </summary>
		/// <param name="db_path">数据库路径</param>
		/// <returns>数据库本身，失败返回 nullptr</returns>
		static Depository_Shared createDepository( const QString &db_path );
		/// <summary>
		/// 使用路径创建一个 sqlite 数据库
		/// </summary>
		/// <param name="db_path">数据库路径</param>
		/// <param name="user">打开数据库时使用的用户名</param>
		/// <param name="password">打开数据库时使用的用户密码</param>
		/// <returns>数据库本身，失败返回 nullptr</returns>
		static Depository_Shared createDepository( const QString &db_path, const QString &user, const QString &password );
	};
}


#endif // SQLITEDEPOSITORY_H_H_HEAD__FILE__
