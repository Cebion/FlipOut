#include <unistd.h>
#include <msp/core/systemerror.h>
#include "handle_private.h"
#include "pipe.h"

namespace Msp {
namespace IO {

void Pipe::platform_init()
{
	int pipe_fd[2];
	if(pipe(pipe_fd)==-1)
		throw system_error("pipe");

	*read_handle = pipe_fd[0];
	*write_handle = pipe_fd[1];
}

} // namespace IO
} // namespace Msp
