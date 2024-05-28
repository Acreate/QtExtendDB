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
		/// 重命名表
		/// </summary>
		/// <param name="new_name">新名</param>
		/// <returns>成功返回 true</returns>
		virtual bool reName( const QString &new_name ) =0;
		/// <summary>
		/// 复制表到另外一个表上
		/// </summary>
		/// <param name="other_tab">目标表</param>
		/// <returns>成功返回 true</returns>
		virtual bool copyTo( const I_TabInfo *other_tab ) const =0;
		/// <summary>
		/// 删除该表
		/// </summary>
		/// <returns>成功返回 true</returns>
		virtual bool removeTab( ) = 0;
		/// <summary>
		/// 校验该对象是否有效
		/// </summary>
		/// <returns>有效返回 true</returns>
		virtual bool isValid( ) const =0;
	};

}


#endif // I_TABINFO_H_H_HEAD__FILE__
