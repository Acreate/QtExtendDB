#ifndef I_TABINFO_H_H_HEAD__FILE__
#define I_TABINFO_H_H_HEAD__FILE__
#pragma once
#include <qstring.h>

#include "../../../nameSpace/cylDB.h"
namespace cylDB {
	class QTEXTENDDB_EXPORT I_TabInfo {
	public:
		virtual ~I_TabInfo( ) { }
	public: // - 接口
		/// <summary>
		/// 获取名称
		/// </summary>
		/// <returns></returns>
		virtual QString getName( ) const = 0;
		/// <summary>
		/// 获取表信息
		/// </summary>
		/// <returns>表信息返回</returns>
		virtual Vector_IResultInfoSPtr_Shared getInfo( ) const = 0;
	};

}


#endif // I_TABINFO_H_H_HEAD__FILE__
