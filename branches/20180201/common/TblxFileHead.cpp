#include "TblxFileHead.h"

TblxFileHead::TblxFileHead(void)
	: version( 20130114 )
	, desc_seg_len( 0 )
	, data_seg_len( 0 )
	, str_seg_len( 0 )
{
	memcpy( head_tips, "tblx", sizeof( head_tips ) );
}

TblxFileHead::~TblxFileHead(void)
{
}

