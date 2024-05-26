#ifndef CYLDB_H_H_HEAD__FILE__
#define CYLDB_H_H_HEAD__FILE__
#pragma once

#include "../auto_generate_files/export/QtExtendDB_export.h"
namespace cylDB {
	/// <summary>
	/// 数据库工具
	/// </summary>
	class QTEXTENDDB_EXPORT DBTools;
	/// <summary>
	/// 数据库接口
	/// </summary>
	class QTEXTENDDB_EXPORT DBInterface;
	/// <summary>
	/// 数据库接口共享指针
	/// </summary>
	using DB_Shared = std::shared_ptr< DBInterface >;
	/// <summary>
	/// 数据库接口基础指针
	/// </summary>
	using DB_Ptr = DBInterface *;
}

#endif // CYLDB_H_H_HEAD__FILE__
