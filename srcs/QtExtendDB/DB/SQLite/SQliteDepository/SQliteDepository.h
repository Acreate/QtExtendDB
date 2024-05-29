#ifndef SQLITEDEPOSITORY_H_H_HEAD__FILE__
#define SQLITEDEPOSITORY_H_H_HEAD__FILE__
#pragma once

#include "../../../nameSpace/cylDB.h"
#include "../../dbInterface/I_DB.h"
#include "../../dbInterface/i_depositoryInterface/I_Depository.h"
#include <QSqlQuery>
#include <QMutex>
namespace cylDB {
	class QTEXTENDDB_EXPORT SQliteDepository : public I_Depository {
	private:
		std::shared_ptr< QSqlDatabase > database; // 数据库链接对象
		QString linkPath; // 链接路径
		QString user; // 用户名称
		QString password; // 用户密码
		std::shared_ptr< QMutex > dbMutex;
		bool isTransaction = false;
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
		/// 删除表
		/// </summary>
		/// <param name="tab_name">表名称</param>
		/// <returns>成功返回 true</returns>
		bool removeTab( const QString &tab_name ) const override;
		/// <summary>
		/// 获取表信息
		/// </summary>
		/// <param name="tab_name">表明</param>
		/// <returns>返回所有列的信息</returns>
		IResultInfo_Shared getTabInfo( const QString &tab_name ) const override;
		/// <summary>
		/// 获取所有表名
		/// </summary>
		/// <returns>结果</returns>
		IResultInfo_Shared getAllTab( ) const override;
		/// <summary>
		/// 在表中创建一项
		/// </summary>
		/// <param name="tab_name">表名称</param>
		/// <param name="item_name">项名</param>
		/// <param name="item_value">值</param>
		/// <param name="where">条件</param>
		/// <returns>成功返回 true</returns>
		bool addItem( const QString &tab_name, const QStringList &item_name, const QStringList &item_value, const QString &where ) const override;
		/// <summary>
		/// 在表中创建一项
		/// </summary>
		/// <param name="tab_name">表名称</param>
		/// <param name="item_name">项名</param>
		/// <param name="item_value">值</param>
		/// <returns>成功返回 true</returns>
		bool addItem( const QString &tab_name, const QStringList &item_name, const QStringList &item_value ) const override;
		/// <summary>
		/// 从表中查找项
		/// </summary>
		/// <param name="tab_name">表名</param>
		/// <param name="item_name">项</param>
		/// <param name="where">条件</param>
		/// <returns>查找结果</returns>
		IResultInfo_Shared findItems( const QString &tab_name, const QStringList &item_name, const QString &where ) const override;
		/// <summary>
		/// 从表中查找项
		/// </summary>
		/// <param name="tab_name">表名</param>
		/// <param name="item_name">项</param>
		/// <returns>查找结果</returns>
		IResultInfo_Shared findItems( const QString &tab_name, const QStringList &item_name ) const override;
		/// <summary>
		/// 获取表的所有项
		/// </summary>
		/// <param name="tab_name">表名</param>
		/// <param name="where">条件</param>
		/// <returns>查找结果</returns>
		IResultInfo_Shared findItems( const QString &tab_name, const QString &where ) const override;
		/// <summary>
		/// 获取表的所有项
		/// </summary>
		/// <param name="tab_name">表名</param>
		/// <returns>查找结果</returns>
		IResultInfo_Shared findItems( const QString &tab_name ) const override;
		/// <summary>
		/// 删除表中的一项
		/// </summary>
		/// <param name="tab_name">表名称</param>
		/// <param name="where">条件</param>
		/// <returns>成功返回 true</returns>
		bool removeItem( const QString &tab_name, const QString &where ) const override;
		/// <summary>
		/// 删除表中所有的项
		/// </summary>
		/// <param name="tab_name">表名</param>
		/// <returns>成功返回 true</returns>
		bool removeItem( const QString &tab_name ) const override;
		/// <summary>
		/// 在表当中更新数据
		/// </summary>
		/// <param name="tab_name">表名称</param>
		/// <param name="var_map_s">数据列表</param>
		/// <param name="where">条件</param>
		/// <returns>成功返回 true</returns>
		bool updateItem( const QString &tab_name, const QVariantMap &var_map_s, const QString &where ) const override;
		/// <summary>
		/// 在表当中更新数据
		/// </summary>
		/// <param name="tab_name">表名称</param>
		/// <param name="var_map_s">数据列表</param>
		/// <returns>成功返回 true</returns>
		bool updateItem( const QString &tab_name, const QVariantMap &var_map_s ) const override;
		/// <summary>
		/// 转换表对象
		/// </summary>
		/// <param name="tab_name">表名</param>
		/// <returns>失败返回 nullptr</returns>
		ITabInfo_Shared converTab( const QString &tab_name ) const override;
		/// <summary>
		/// 转换当前数据库的所有表对象
		/// </summary>
		/// <returns>失败返回 nullptr</returns>
		Vector_ITabInfoSPtr_Shared converAllTab( ) const override;
		/// <summary>
		/// 执行一个sql命令
		/// </summary>
		/// <param name="query">命令</param>
		/// <param name="sql_exec_result">返回</param>
		/// <returns>成功表示为 true</returns>
		bool exec( const QString &query, IResultInfo_Shared &sql_exec_result ) const override;
		/// <summary>
		/// 执行一个 sql 命令
		/// </summary>
		/// <param name="query">执行对象</param>
		/// <param name="sql_exec_result">执行返回</param>
		/// <returns>成功返回 true</returns>
		bool exec( QSqlQuery *query, IResultInfo_Shared &sql_exec_result ) const override;
		/// <summary>
		/// 执行一个 sql 命令
		/// </summary>
		/// <param name="cmd">执行对象</param>
		/// <returns>成功返回 true</returns>
		bool exec( QSqlQuery *cmd ) const override;
		/// <summary>
		/// 生成一个执行器
		/// </summary>
		/// <returns>执行器指针对象，失败返回 nullptr</returns>
		std::shared_ptr< QSqlQuery > generateSqlQuery( ) const override {
			return std::make_shared< QSqlQuery >( *database );
		}
		/// <summary>
		/// 生成一个执行器
		/// </summary>
		/// <returns>执行器指针对象，失败返回 nullptr</returns>
		std::shared_ptr< QSqlQuery > generateSqlQuery( const QString &query ) const override {
			return std::make_shared< QSqlQuery >( query, *database );
		}
		/// <summary>
		/// 提交事务
		/// </summary>
		/// <returns>失败返回 false</returns>
		bool commit( ) override {
			if( isTransaction )
				dbMutex->unlock( );
			isTransaction = false;
			return database->commit( );
		}
		/// <summary>
		/// 开始事务
		/// </summary>
		/// <returns>不支持返回 false</returns>
		bool transaction( ) override {
			isTransaction = database->transaction( );
			if( isTransaction )
				dbMutex->lock( );
			return isTransaction;
		}
		/// <summary>
		/// 回滚事务
		/// </summary>
		/// <returns>失败返回 false</returns>
		bool rollback( ) override {
			if( isTransaction )
				dbMutex->unlock( );
			isTransaction = false;
			return database->rollback( );
		}
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
