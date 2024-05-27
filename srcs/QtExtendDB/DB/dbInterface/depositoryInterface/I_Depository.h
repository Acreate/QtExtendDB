#ifndef I_DEPOSITORY_H_H_HEAD__FILE__
#define I_DEPOSITORY_H_H_HEAD__FILE__
#pragma once
#include <QSqlError>
#include <qstring.h>

#include "../../../nameSpace/cylDB.h"
namespace cylDB {
	class QTEXTENDDB_EXPORT I_Depository {
	public:
		virtual ~I_Depository( ) { }
	public:
		/// <summary>
		/// 获取数据库名称
		/// </summary>
		/// <returns>名称</returns>
		virtual QString getDBName( ) const = 0;
		/// <summary>
		/// 是否存在表
		/// </summary>
		/// <param name="tab_name"></param>
		/// <returns></returns>
		virtual bool hasTab( const QString &tab_name ) const = 0;
		/// <summary>
		/// 创建表
		/// </summary>
		/// <param name="tab_name">表名称</param>
		/// <param name="tab_info">表信息</param>
		/// <param name="key">关键 key</param>
		/// <param name="key_type">key 的类型</param>
		/// <returns>成功返回 true</returns>
		virtual bool createTab( const QString &tab_name, const QVariantMap &tab_info, const QString &key , const QString &key_type) const = 0;
		/// <summary>
		/// 创建表
		/// </summary>
		/// <param name="tab_name">表名称</param>
		/// <param name="tab_info">表信息</param>
		/// <returns>成功返回 true</returns>
		virtual bool createTab( const QString &tab_name, const QVariantMap &tab_info ) const = 0;
		/// <summary>
		/// 获取表信息
		/// </summary>
		/// <param name="tab_name">表明</param>
		/// <returns>返回所有列的信息</returns>
		virtual QMap< QVariant, QVariant > getTabInfo( const QString &tab_name ) const = 0;
		/// <summary>
		/// 在表中创建一项
		/// </summary>
		/// <param name="tab_name">表名称</param>
		/// <param name="item_name">项名</param>
		/// <returns>成功返回 true</returns>
		virtual bool addItem( const QString &tab_name, const QString &item_name ) const = 0;
		/// <summary>
		/// 删除表中的一项
		/// </summary>
		/// <param name="tab_name">表名称</param>
		/// <param name="item_name">项名</param>
		/// <returns>成功返回 true</returns>
		virtual bool removeItem( const QString &tab_name, const QString &item_name ) const = 0;
		/// <summary>
		/// 在表当中插入数据
		/// </summary>
		/// <param name="tab_name">表名称</param>
		/// <param name="variants">数据列表</param>
		/// <returns>成功返回 true</returns>
		virtual bool appendData( const QString &tab_name, QVector< QVariant > variants ) const = 0;
		/// <summary>
		/// 在表当中更新数据
		/// </summary>
		/// <param name="tab_name">表名称</param>
		/// <param name="var_map_s">数据列表</param>
		/// <returns>成功返回 true</returns>
		virtual bool updateData( const QString &tab_name, QMap< QVariant, QVariant > var_map_s ) const = 0;
		/// <summary>
		/// 删除表中信息，满足条件为完整匹配 var_map_s 参数
		/// </summary>
		/// <param name="tab_name">表名称</param>
		/// <param name="var_map_s">数据列表</param>
		/// <returns>成功返回 true</returns>
		virtual bool removeData( const QString &tab_name, QMap< QVariant, QVariant > var_map_s ) const = 0;
		/// <summary>
		/// 设置用户信息
		/// </summary>
		/// <param name="user">用户名称</param>
		/// <param name="password">用户密码</param>
		virtual void setUserInfo( const QString &user, const QString &password ) = 0;
		/// <summary>
		/// 是否已经打开
		/// </summary>
		/// <returns>打开状态</returns>
		virtual bool isOpen( ) const = 0;
		/// <summary>
		/// 打开数据库
		/// </summary>
		/// <returns>成功返回 true</returns>
		virtual bool open( ) const = 0;
		/// <summary>
		/// 打开数据库
		/// </summary>
		/// <returns>成功返回 true</returns>
		virtual bool open( const QString &user, const QString &password ) = 0;
		/// <summary>
		/// 关闭数据库
		/// </summary>
		/// <returns>成功返回 true</returns>
		virtual bool close( ) const = 0;
		/// <summary>
		/// 获取最后的错误
		/// </summary>
		/// <returns>最后的错误</returns>
		virtual QSqlError getLastError( ) const = 0;
		/// <summary>
		/// 提交
		/// </summary>
		/// <returns>成功返回 true</returns>
		virtual bool commit( ) const = 0;
		/// <summary>
		/// 是否打开错误
		/// </summary>
		/// <returns>打开错误返回 true</returns>
		virtual bool isOpenError( ) const = 0;
	};
}
#endif // I_DEPOSITORY_H_H_HEAD__FILE__
