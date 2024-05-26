#ifndef CYLDB_H_H_HEAD__FILE__
#define CYLDB_H_H_HEAD__FILE__
#pragma once

#include "../auto_generate_files/export/QtExtendDB_export.h"
#include <memory>
namespace cylDB {
	/// <summary>
	/// 数据库工具
	/// </summary>
	class QTEXTENDDB_EXPORT DBTools;
	/// <summary>
	/// 数据库接口
	/// </summary>
	class QTEXTENDDB_EXPORT I_DB;
	/// <summary>
	/// 数据库类
	/// </summary>
	class QTEXTENDDB_EXPORT I_Depository;
	/// <summary>
	/// 校验串
	/// </summary>
	class QTEXTENDDB_EXPORT DataCheck;
	/// <summary>
	/// 序列化
	/// </summary>
	class QTEXTENDDB_EXPORT Serialization;
	/// <summary>
	/// 反序列化
	/// </summary>
	class QTEXTENDDB_EXPORT Unserialize;
	/// <summary>
	/// 数据库接口共享指针
	/// </summary>
	using DB_Shared = std::shared_ptr< I_DB >;
	/// <summary>
	/// 数据库接口基础指针
	/// </summary>
	using DB_Ptr = I_DB *;
	/// <summary>
	/// 数据列
	/// </summary>
	using Data_Array = std::shared_ptr< uint8_t[ ] >;
}

#endif // CYLDB_H_H_HEAD__FILE__
