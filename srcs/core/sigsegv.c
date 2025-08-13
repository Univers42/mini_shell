#include <signal.h>
#include <unistd.h>
#include <stdint.h>
#include <ucontext.h>
#if defined(__linux__)
# include <sys/ucontext.h>
# include <fcntl.h>
#endif
#include "minishell.h"

// signal-safe write that consumes all bytes or stops on error
static void	safe_write(int fd, const char *buf, size_t len)
{
	while (len > 0)
	{
		ssize_t wr = write(fd, buf, len);
		if (wr <= 0)
			break;
		buf += (size_t)wr;
		len -= (size_t)wr;
	}
}

static void	safe_puts(int fd, const char *s)
{
	size_t len = 0;
	while (s[len])
		len++;
	safe_write(fd, s, len);
}

static void	safe_puthex_u64(int fd, unsigned long v)
{
	char buf[2 + (sizeof(unsigned long) * 2) + 1];
	const char *hex = "0123456789abcdef";
	size_t i = sizeof(buf);
	buf[--i] = '\0';
	if (v == 0)
		buf[--i] = '0';
	else
	{
		while (v && i > 2)
		{
			buf[--i] = hex[v & 0xF];
			v >>= 4;
		}
	}
	buf[--i] = 'x';
	buf[--i] = '0';
	safe_write(fd, &buf[i], ((sizeof(buf) - 1) - i));
}

// Provided by libft debug utilities
void	*ft_print_memory(void *addr, unsigned int size);

// best-effort hexdump around fault address (not fully async-signal-safe)
static void	debug_hexdump_fault(void *fault_addr)
{
	unsigned char	*start;
	unsigned long	addr = (unsigned long)fault_addr;

	start = (addr < 32ul) ? (unsigned char *)0 : (unsigned char *)(addr - 32ul);
	safe_puts(STDERR_FILENO, "\n[hexdump] around ");
	safe_puthex_u64(STDERR_FILENO, (unsigned long)fault_addr);
	safe_puts(STDERR_FILENO, " (best-effort)\n");
	ft_print_memory((void *)start, 96);
	safe_puts(STDERR_FILENO, "\n");
}

static void	dump_proc_maps(void)
{
#if defined(__linux__)
	int		fd;
	char	buf[4096];
	ssize_t	rd;

	safe_puts(STDERR_FILENO, "[/proc/self/maps]\n");
	fd = open("/proc/self/maps", O_RDONLY);
	if (fd < 0)
		return;
	while ((rd = read(fd, buf, sizeof(buf))) > 0)
		safe_write(STDERR_FILENO, buf, (size_t)rd);
	close(fd);
	safe_puts(STDERR_FILENO, "\n");
#else
	safe_puts(STDERR_FILENO, "[maps] not available on this platform\n");
#endif
}

static void	print_siginfo(int sig, siginfo_t *info)
{
	safe_puts(STDERR_FILENO, "  signal:     ");
	safe_puthex_u64(STDERR_FILENO, (unsigned long)sig);
	safe_puts(STDERR_FILENO, "  si_code: ");
	safe_puthex_u64(STDERR_FILENO, (unsigned long)info->si_code);
	safe_puts(STDERR_FILENO, "  errno: ");
	safe_puthex_u64(STDERR_FILENO, (unsigned long)info->si_errno);
	safe_puts(STDERR_FILENO, "\n");
}

static void	ms_on_sigsegv(int sig, siginfo_t *info, void *uctx)
{
	(void)sig;
	safe_puts(STDERR_FILENO, "\n[SEGFAULT] minishell crashed\n");
	print_siginfo(SIGSEGV, info);
	safe_puts(STDERR_FILENO, "  fault addr: ");
	safe_puthex_u64(STDERR_FILENO, (unsigned long)info->si_addr);
	safe_puts(STDERR_FILENO, "\n");

#if (defined(__x86_64__) || defined(__i386__)) && (defined(__linux__) || defined(__GLIBC__))
	if (uctx)
	{
# if defined(__x86_64__)
#  if defined(REG_RIP) || defined(REG_RSP)
		ucontext_t *uc = (ucontext_t *)uctx;
#   if defined(REG_RIP)
		unsigned long rip = (unsigned long)uc->uc_mcontext.gregs[REG_RIP];
		safe_puts(STDERR_FILENO, "  RIP:        ");
		safe_puthex_u64(STDERR_FILENO, rip);
		safe_puts(STDERR_FILENO, "\n");
#   endif
#   if defined(REG_RSP)
		unsigned long rsp = (unsigned long)uc->uc_mcontext.gregs[REG_RSP];
		safe_puts(STDERR_FILENO, "  RSP:        ");
		safe_puthex_u64(STDERR_FILENO, rsp);
		safe_puts(STDERR_FILENO, "\n");
#   endif
#  else
		safe_puts(STDERR_FILENO, "  ucontext:   ");
		safe_puthex_u64(STDERR_FILENO, (unsigned long)uctx);
		safe_puts(STDERR_FILENO, "\n");
#  endif
# elif defined(__i386__)
#  if defined(REG_EIP) || defined(REG_ESP)
		ucontext_t *uc = (ucontext_t *)uctx;
#   if defined(REG_EIP)
		unsigned long eip = (unsigned long)uc->uc_mcontext.gregs[REG_EIP];
		safe_puts(STDERR_FILENO, "  EIP:        ");
		safe_puthex_u64(STDERR_FILENO, eip);
		safe_puts(STDERR_FILENO, "\n");
#   endif
#   if defined(REG_ESP)
		unsigned long esp = (unsigned long)uc->uc_mcontext.gregs[REG_ESP];
		safe_puts(STDERR_FILENO, "  ESP:        ");
		safe_puthex_u64(STDERR_FILENO, esp);
		safe_puts(STDERR_FILENO, "\n");
#   endif
#  else
		safe_puts(STDERR_FILENO, "  ucontext:   ");
		safe_puthex_u64(STDERR_FILENO, (unsigned long)uctx);
		safe_puts(STDERR_FILENO, "\n");
#  endif
# endif
	}
#endif

	debug_hexdump_fault(info->si_addr);
	dump_proc_maps();
	safe_puts(STDERR_FILENO, "[SEGFAULT] exiting (code 139)\n");
	_exit(139);
}

void	ms_install_segv_handler(void)
{
	struct sigaction sa;

	sa.sa_sigaction = &ms_on_sigsegv;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGSEGV, &sa, NULL);
}
