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
		virtual bool link() = 0;
		/// <summary>
		/// 是否已经链接
		/// </summary>
		/// <returns>成功返回 true</returns>
		virtual bool isLink() = 0;
		/// <summary>
		/// 是否存在数据库
		/// </summary>
		/// <param name="db_name">数据库名称</param>
		/// <returns>存在返回 true</returns>
		virtual bool hasDepository(const QString& db_name) = 0;
	};
}


#endif // DBINTERFACE_H_H_HEAD__FILE__
