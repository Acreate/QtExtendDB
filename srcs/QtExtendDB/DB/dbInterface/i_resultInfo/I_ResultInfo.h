#ifndef I_RESULTINFO_H_H_HEAD__FILE__
#define I_RESULTINFO_H_H_HEAD__FILE__
#pragma once
#include <qvariant.h>

#include "../../../nameSpace/cylDB.h"
namespace cylDB {
	class QTEXTENDDB_EXPORT I_ResultInfo {

	public:
		virtual ~I_ResultInfo( ) { };
	public: // - 接口
		/// <summary>
		/// 获取返回个数 行 * 列
		/// </summary>
		/// <returns>返回个数</returns>
		virtual size_t getCount( ) const = 0;
		/// <summary>
		/// 获取列数
		/// </summary>
		/// <returns>列数</returns>
		virtual size_t getColCount( ) const = 0;
		/// <summary>
		/// 获取指定行
		/// </summary>
		/// <param name="index">行下标，从 0 开始</param>
		/// <returns>失败返回 nullptr</returns>
		virtual const Vector_VarSPtr_Shared getRows( const size_t index ) const = 0;

		/// <summary>
		/// 根据行列进行数值返回
		/// </summary>
		/// <param name="row">行</param>
		/// <param name="col">列</param>
		/// <returns>值</returns>
		virtual const Var_Shared get( const size_t row, const size_t col ) const = 0;
		/// <summary>
		/// 获取当前行
		/// </summary>
		/// <returns>失败返回 nullptr</returns>
		virtual const Vector_VarSPtr_Shared getCurrentRows( ) const = 0;
		/// <summary>
		/// 清空当前行
		/// </summary>
		virtual void clearCurrentCol( ) = 0;
		/// <summary>
		/// 获取当前下标
		/// </summary>
		/// <returns>当前下标</returns>
		virtual size_t currentIndex( ) const = 0;
		/// <summary>
		/// 重置下标
		/// </summary>
		virtual void resetColIndex( ) = 0;
		/// <summary>
		/// 清空整个对象
		/// </summary>
		virtual void clear( ) = 0;
		/// <summary>
		/// 下一行
		/// </summary>
		virtual bool nextCol( ) = 0;
		/// <summary>
		/// 当前列
		/// </summary>
		/// <returns>列数</returns>
		virtual size_t currentCol( ) = 0;
	};
}

#endif // I_RESULTINFO_H_H_HEAD__FILE__
