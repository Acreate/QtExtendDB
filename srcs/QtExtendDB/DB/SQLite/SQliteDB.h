#ifndef SQLITE_H_H_HEAD__FILE__
#define SQLITE_H_H_HEAD__FILE__
#pragma once
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
		Depository_Shared openDepository( const QString &db_name ) override;
		/// <summary>
		/// 获取链接当中的所有数据库
		/// </summary>
		/// <returns>数据库列表</returns>
		QVector< Depository_Shared > getDepositorys( ) override;
	};
}


#endif // SQLITE_H_H_HEAD__FILE__
