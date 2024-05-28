#ifndef SQLITE_H_H_HEAD__FILE__
#define SQLITE_H_H_HEAD__FILE__
#pragma once
#include <QDir>

#include "../../nameSpace/cylDB.h"
#include "../dbInterface/I_DB.h"
namespace cylDB {
	class QTEXTENDDB_EXPORT SQliteDB : public I_DB {
		QString linkPath; // 链接信息
	public:
		explicit SQliteDB( const QString &link );
		~SQliteDB( ) override;
		/// <summary>
		/// 链接
		/// </summary>
		/// <returns>成功返回 true</returns>
		bool link( ) override;
		/// <summary>
		/// 链接指定链接
		/// </summary>
		/// <param name="link">链接</param>
		/// <returns>成功返回 true</returns>
		bool link( const QString &link ) override;

		/// <summary>
		/// 返回链接
		/// </summary>
		/// <returns>链接</returns>
		QString getLink( ) override;
		/// <summary>
		/// 是否已经链接
		/// </summary>
		/// <returns>成功返回 true</returns>
		bool isLink( ) override;
		/// <summary>
		/// 是否存在数据库
		/// </summary>
		/// <param name="db_name">数据库名称</param>
		/// <returns>存在返回 true</returns>
		bool hasDepository( const QString &db_name ) override;
		/// <summary>
		/// 打开数据库
		/// </summary>
		/// <param name="db_name">数据库名称</param>
		/// <returns>返回数据库对象指针，失败返回 nullptr</returns>
		Depository_Shared openDepository( const QString &db_name ) override {
			auto dbPath = linkPath + QDir::separator( ) + db_name;
			return openAbsoluteFilePathDepository( dbPath );
		}
		/// <summary>
		/// 不使用本地的 link 路径实现数据库打开
		/// </summary>
		/// <param name="absoluteFilePath">数据库名称</param>
		/// <returns>返回数据库对象指针，失败返回 nullptr</returns>
		Depository_Shared openAbsoluteFilePathDepository( const QString &absoluteFilePath ) override;
		/// <summary>
		/// 打开数据库
		/// </summary>
		/// <param name="db_name">数据库名称</param>
		/// <param name="name">打开数据库时用的用户名</param>
		/// <param name="password">打开数据库时用的用户密码</param>
		/// <returns>返回数据库对象指针，失败返回 nullptr</returns>
		Depository_Shared openDepository( const QString &db_name, const QString &name, const QString &password ) const override {
			auto dbPath = linkPath + QDir::separator( ) + db_name;
			return openAbsoluteFilePathDepository( dbPath, name, password );
		}
		/// <summary>
		/// 不使用本地的 link 路径实现数据库打开
		/// </summary>
		/// <param name="absoluteFilePath">数据库名称</param>
		/// <param name="name">打开数据库时用的用户名</param>
		/// <param name="password">打开数据库时用的用户密码</param>
		/// <returns>返回数据库对象指针，失败返回 nullptr</returns>
		Depository_Shared openAbsoluteFilePathDepository( const QString &absoluteFilePath, const QString &name, const QString &password ) const override;
		/// <summary>
		/// 获取链接当中的所有数据库
		/// </summary>
		/// <returns>数据库列表</returns>
		QVector< Depository_Shared > getDepositorys( ) override;
		/// <summary>
		/// 返回支持类型
		/// </summary>
		/// <returns>支持类型映射，使用 typeid( type ) 作为关键字</returns>
		QVariantMap getSupportType( ) const override {
			return { { typeid( int8_t ).name( ), "INTEGER" }
				, { typeid( int16_t ).name( ), "INTEGER" }
				, { typeid( int32_t ).name( ), "INTEGER" }
				, { typeid( uint8_t ).name( ), "INTEGER" }
				, { typeid( uint16_t ).name( ), "INTEGER" }
				, { typeid( uint32_t ).name( ), "INTEGER" }
				, { typeid( QChar ).name( ), "INTEGER" }
				, { typeid( bool ).name( ), "BLOB" }
				, { "byte", "BLOB" }
				, { typeid( std::string ).name( ), "TEXT" }
				, { typeid( std::wstring ).name( ), "TEXT" }
				, { typeid( QString ).name( ), "TEXT" }
				, { typeid( char * ).name( ), "TEXT" }
				, { typeid( QChar * ).name( ), "TEXT" }
				, { typeid( char [ ] ).name( ), "TEXT" }
				, { typeid( QChar [ ] ).name( ), "TEXT" }
				, { "any_num", "NUMERIC" }
				, { typeid( float ).name( ), "REAL" }
				, { typeid( double ).name( ), "REAL" }
				, { typeid( void * ).name( ), "NULL" }
				, { typeid( std::time_t ).name( ), "DATETIME" }
				, { typeid( QDate ).name( ), "DATE" }
				, { typeid( QTime ).name( ), "TIME" }
				, { typeid( QDateTime ).name( ), "DATETIME" }
			};
		}
	};
}


#endif // SQLITE_H_H_HEAD__FILE__
