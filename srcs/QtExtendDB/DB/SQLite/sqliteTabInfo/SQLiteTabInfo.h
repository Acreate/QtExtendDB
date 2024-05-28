#ifndef SQLITETABINFO_H_H_HEAD__FILE__
#define SQLITETABINFO_H_H_HEAD__FILE__
#pragma once

#include "../../../nameSpace/cylDB.h"
#include "../../dbInterface/i_tabInfo/I_TabInfo.h"
namespace cylDB {
	class QTEXTENDDB_EXPORT SQLiteTabInfo : public I_TabInfo {
		using Release_Call = std::function< void( QSqlDatabase **database_ptr ) >;
		friend class SQliteDepository;
	private: // - 私有成员
		QSqlDatabase *database; // 依赖数据库
		Release_Call releaseCall; // 释放调用的函数
		QMutex *dbMutex; // 数据库锁
	private: // - 构造函数
		/// <summary>
		/// 构建一个表
		/// </summary>
		/// <param name="db_mutex">数据库引用的锁</param>
		/// <param name="release_call">释放调用函数</param>
		SQLiteTabInfo( QMutex *db_mutex, Release_Call release_call ) : dbMutex( db_mutex ), releaseCall( release_call ) {
		}
		/// <summary>
		/// 构建一个表
		/// </summary>
		/// <param name="db_mutex">数据库引用的锁</param>
		SQLiteTabInfo( QMutex *db_mutex ) : dbMutex( db_mutex ) {
			releaseCall = []( QSqlDatabase **database_ptr ) {
				*database_ptr = nullptr;
			};
		}

	public:
		~SQLiteTabInfo( ) override {
			releaseCall( &database );
		}
		QString getName( ) const override;

		bool reName( const QString &new_name ) override;
		bool copyTo( const I_TabInfo *other_tab ) const override;
		bool removeTab( ) override;
		bool isValid( ) const override;
	};
}


#endif // SQLITETABINFO_H_H_HEAD__FILE__
