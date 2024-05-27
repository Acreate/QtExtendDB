#ifndef SQLITETABINFO_H_H_HEAD__FILE__
#define SQLITETABINFO_H_H_HEAD__FILE__
#pragma once

#include "../../../nameSpace/cylDB.h"
#include "../../dbInterface/i_tabInfo/I_TabInfo.h"
namespace cylDB {
	class QTEXTENDDB_EXPORT SQLiteTabInfo : public I_TabInfo {
		friend class SQliteDepository;
		SQLiteTabInfo( ) { }
	public:
		QString getName( ) const override;
		Vector_IResultInfoSPtr_Shared getInfo( ) const override;
	};
}


#endif // SQLITETABINFO_H_H_HEAD__FILE__
