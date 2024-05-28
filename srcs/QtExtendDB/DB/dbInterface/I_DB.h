#ifndef I_DB_H_H_HEAD__FILE__
#define I_DB_H_H_HEAD__FILE__
#pragma once
#include <qstring.h>

#include "../../nameSpace/cylDB.h"
namespace cylDB {
	class QTEXTENDDB_EXPORT I_DB {
	public:
		I_DB( ) { }
		virtual ~I_DB( ) { }
	public: // - 接口
		/// <summary>
		/// 链接
		/// </summary>
		/// <returns>成功返回 true</returns>
		virtual bool link( ) = 0;
		/// <summary>
		/// 链接指定链接
		/// </summary>
		/// <param name="link">链接</param>
		/// <returns>成功返回 true</returns>
		virtual bool link( const QString &link ) = 0;
		/// <summary>
		/// 返回链接
		/// </summary>
		/// <returns>链接</returns>
		virtual QString getLink( ) = 0;
		/// <summary>
		/// 是否已经链接
		/// </summary>
		/// <returns>成功返回 true</returns>
		virtual bool isLink( ) = 0;
		/// <summary>
		/// 是否存在数据库
		/// </summary>
		/// <param name="db_name">数据库名称</param>
		/// <returns>存在返回 true</returns>
		virtual bool hasDepository( const QString &db_name ) = 0;
		/// <summary>
		/// 打开数据库
		/// </summary>
		/// <param name="db_name">数据库名称</param>
		/// <returns>返回数据库对象指针，失败返回 nullptr</returns>
		virtual Depository_Shared openDepository( const QString &db_name ) = 0;
		/// <summary>
		/// 不使用本地的 link 路径实现数据库打开
		/// </summary>
		/// <param name="absoluteFilePath">数据库名称</param>
		/// <returns>返回数据库对象指针，失败返回 nullptr</returns>
		virtual Depository_Shared openAbsoluteFilePathDepository( const QString &absoluteFilePath ) = 0;
		/// <summary>
		/// 打开数据库
		/// </summary>
		/// <param name="db_name">数据库名称</param>
		/// <param name="name">打开数据库时用的用户名</param>
		/// <param name="password">打开数据库时用的用户密码</param>
		/// <returns>返回数据库对象指针，失败返回 nullptr</returns>
		virtual Depository_Shared openDepository( const QString &db_name, const QString &name, const QString &password ) const = 0;
		/// <summary>
		/// 不使用本地的 link 路径实现数据库打开
		/// </summary>
		/// <param name="absoluteFilePath">数据库名称</param>
		/// <param name="name">打开数据库时用的用户名</param>
		/// <param name="password">打开数据库时用的用户密码</param>
		/// <returns>返回数据库对象指针，失败返回 nullptr</returns>
		virtual Depository_Shared openAbsoluteFilePathDepository( const QString &absoluteFilePath, const QString &name, const QString &password ) const = 0;
		/// <summary>
		/// 获取链接当中的所有数据库
		/// </summary>
		/// <returns>数据库列表</returns>
		virtual QVector< Depository_Shared > getDepositorys( ) = 0;
		/// <summary>
		/// 返回支持类型
		/// </summary>
		/// <returns>支持类型映射，使用 typeid( type ) 作为关键字</returns>
		virtual QVariantMap getSupportType( ) const = 0;
	};
}


#endif // DBINTERFACE_H_H_HEAD__FILE__
