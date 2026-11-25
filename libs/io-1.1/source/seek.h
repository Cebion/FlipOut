/* $Id$

This file is part of libmspio
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/
#ifndef MSP_IO_SEEK_H_
#define MSP_IO_SEEK_H_

namespace Msp {
namespace IO {

enum SeekType
{
	S_BEG,
	S_CUR,
	S_END
};

extern int sys_seek_type(SeekType);

} // namespace IO
} // namespace Msp

#endif
