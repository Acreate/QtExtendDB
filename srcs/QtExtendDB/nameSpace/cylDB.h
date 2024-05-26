#ifndef CYLDB_H_H_HEAD__FILE__
#define CYLDB_H_H_HEAD__FILE__
#pragma once

#include "../auto_generate_files/export/QtExtendDB_export.h"
namespace cylDB {
	/// <summary>
	/// ���ݿ⹤��
	/// </summary>
	class QTEXTENDDB_EXPORT DBTools;
	/// <summary>
	/// ���ݿ�ӿ�
	/// </summary>
	class QTEXTENDDB_EXPORT DBInterface;
	/// <summary>
	/// ���ݿ�ӿڹ���ָ��
	/// </summary>
	using DB_Shared = std::shared_ptr< DBInterface >;
	/// <summary>
	/// ���ݿ�ӿڻ���ָ��
	/// </summary>
	using DB_Ptr = DBInterface *;
}

#endif // CYLDB_H_H_HEAD__FILE__
