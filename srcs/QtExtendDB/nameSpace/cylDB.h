#ifndef CYLDB_H_H_HEAD__FILE__
#define CYLDB_H_H_HEAD__FILE__
#pragma once

#include "../auto_generate_files/export/QtExtendDB_export.h"
#include <QVariant>
#include <vector>
#include <memory>
#include <unordered_map>
namespace cylDB {
	/// <summary>
	/// 数据库接口
	/// </summary>
	class QTEXTENDDB_EXPORT I_DB;
	/// <summary>
	/// 数据库类
	/// </summary>
	class QTEXTENDDB_EXPORT I_Depository;
	/// <summary>
	/// 表信息
	/// </summary>
	class QTEXTENDDB_EXPORT I_TabInfo;
	/// <summary>
	/// 返回信息
	/// </summary>
	class QTEXTENDDB_EXPORT I_ResultInfo;
	/// <summary>
	/// 数据库工具
	/// </summary>
	class QTEXTENDDB_EXPORT DBTools;
	/// <summary>
	/// sqlite 链接类
	/// </summary>
	class QTEXTENDDB_EXPORT SQliteDB;
	/// <summary>
	/// sqlite 数据库对象
	/// </summary>
	class QTEXTENDDB_EXPORT SQliteDepository;
	/// <summary>
	/// sqlite 列
	/// </summary>
	class QTEXTENDDB_EXPORT SQLiteColInfo;
	/// <summary>
	/// sqlite 返回结果
	/// </summary>
	class QTEXTENDDB_EXPORT SQLiteResult;
	/// <summary>
	/// sqlite 行
	/// </summary>
	class QTEXTENDDB_EXPORT SQLiteRowInfo;
	/// <summary>
	/// sqlite 表信息
	/// </summary>
	class QTEXTENDDB_EXPORT SQLiteTabInfo;
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
	/// 数值指针
	/// </summary>
	using Var_Ptr = QVariant *;
	/// <summary>
	/// 数值共享指针
	/// </summary>
	using Var_Shared = std::shared_ptr< QVariant >;
	/// <summary>
	/// 数值共享指针列表
	/// </summary>
	using Vector_VarSPtr = std::vector< Var_Shared >;
	/// <summary>
	/// 数值共享指针列表共享指针
	/// </summary>
	using Vector_VarSPtr_Shared = std::shared_ptr< Vector_VarSPtr >;
	/// <summary>
	/// 表映射
	/// </summary>
	using UN_Tab_Map = std::unordered_map< size_t, Vector_VarSPtr_Shared >;
	/// <summary>
	/// 表映射共享指针对象
	/// </summary>
	using UNTabMap_Shared = std::shared_ptr< UN_Tab_Map >;
	/// <summary>
	/// 数据库接口共享指针
	/// </summary>
	using DB_Shared = std::shared_ptr< I_DB >;
	/// <summary>
	/// 数据库对象共享指针
	/// </summary>
	using Depository_Shared = std::shared_ptr< I_Depository >;

	/// <summary>
	/// 表对象指针
	/// </summary>
	using ITabInfo_Shared = std::shared_ptr< I_TabInfo >;
	/// <summary>
	/// 表对象指针列表对象
	/// </summary>
	using Vector_ITabInfoSPtr = std::vector< ITabInfo_Shared >;
	/// <summary>
	/// 表对象指针列表对象指针
	/// </summary>
	using Vector_ITabInfoSPtr_Shared = std::shared_ptr< Vector_ITabInfoSPtr >;
	/// <summary>
	/// 映射名称表指针对象
	/// </summary>
	using UN_QStringK_ITabInfoSPtrV_Map = std::unordered_map< QString, ITabInfo_Shared >;
	/// <summary>
	/// 映射名称表指针对象指针
	/// </summary>
	using UN_QStringK_ITabInfoSPtrV_Map_Shared = std::shared_ptr< UN_QStringK_ITabInfoSPtrV_Map >;

	/// <summary>
	/// 数据库接口基础指针
	/// </summary>
	using DB_Ptr = I_DB *;
	/// <summary>
	/// 数据列
	/// </summary>
	using Data_Array = std::shared_ptr< uint8_t[ ] >;


	/// <summary>
	/// 返回接口对象指针
	/// </summary>
	using IResultInfo_Shared = std::shared_ptr< I_ResultInfo >;
	/// <summary>
	/// 返回口指针对象数组
	/// </summary>
	using Vector_IResultInfoSPtr = std::vector< IResultInfo_Shared >;
	/// <summary>
	/// 返回口指针对象数组指针
	/// </summary>
	using Vector_IResultInfoSPtr_Shared = std::shared_ptr< Vector_IResultInfoSPtr >;


}

#endif // CYLDB_H_H_HEAD__FILE__
