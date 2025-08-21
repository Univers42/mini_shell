/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danielm3 <danielm3@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 20:03:10 by danielm3          #+#    #+#             */
/*   Updated: 2025/08/21 21:26:37 by danielm3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _POSIX_C_SOURCE 200809L
#include "signals.h"
#include "minishell.h"
#include <signal.h>
#include <string.h>

/**
 * @brief Configure process signal dispositions/handlers.
 *
 * Sets the disposition for SIGINT and/or SIGQUIT according to bitmask flags.
 * For each selected signal in @p signal_mask, the mode is chosen from
 * @p mode_mask: DEFAULT → SIG_DFL, IGNORE → SIG_IGN, or install a custom
 * handler (INTERACTIVE / MONITOR). Unselected signals are left unchanged.
 *
 * @param signal_mask  Bitmask selecting signals to configure.
 *                     Use SIG_MASK_INT for SIGINT, SIG_MASK_QUIT for SIGQUIT.
 *                     You may OR them together.
 * @param mode_mask    Bitmask choosing the mode to apply to selected signals.
 *                     One of: MODE_MASK_DEFAULT, MODE_MASK_IGNORE,
 *                     MODE_MASK_INTERACTIVE (SIGINT only),
 *                     MODE_MASK_MONITOR.
 * @param context_mask Context flags for future behavior customization.
 *                     (Currently unused; ignored.)
 *
 * @return int         0 on success; non-zero if any underlying call fails.
 *                     The return value is the bitwise OR of the error codes
 *                     from signal_set_disposition()/signal_install().
 *
 * @note MODE_MASK_INTERACTIVE installs sigint_interactive_handler for SIGINT.
 * @note MODE_MASK_MONITOR installs sigint_monitor_handler (SIGINT) or
 *       sigquit_monitor_handler (SIGQUIT).
 * @warning Signal disposition is process-wide and affects all threads.
 *          Call only from initialization or with care.
 */
/**
 * @brief Configure process signal dispositions/handlers.
 *
 * Sets the disposition for SIGINT and/or SIGQUIT according to bitmask flags.
 * For each selected signal in @p signal_mask, the mode is chosen from
 * @p mode_mask: DEFAULT → SIG_DFL, IGNORE → SIG_IGN, or install a custom
 * handler (INTERACTIVE / MONITOR). Unselected signals are left unchanged.
 *
 * @param signal_mask  Bitmask selecting signals to configure.
 *                     Use SIG_MASK_INT for SIGINT, SIG_MASK_QUIT for SIGQUIT.
 *                     You may OR them together.
 * @param mode_mask    Bitmask choosing the mode to apply to selected signals.
 *                     One of: MODE_MASK_DEFAULT, MODE_MASK_IGNORE,
 *                     MODE_MASK_INTERACTIVE (SIGINT only),
 *                     MODE_MASK_MONITOR.
 * @param context_mask Context flags for future behavior customization.
 *                     (Currently unused; ignored.)
 *
 * @return int         0 on success; non-zero if any underlying call fails.
 *                     The return value is the bitwise OR of the error codes
 *                     from signal_set_disposition()/signal_install().
 *
 * @note MODE_MASK_INTERACTIVE installs sigint_interactive_handler for SIGINT.
 * @note MODE_MASK_MONITOR installs sigint_monitor_handler (SIGINT) or
 *       sigquit_monitor_handler (SIGQUIT).
 * @warning Signal disposition is process-wide and affects all threads.
 *          Call only from initialization or with care.
 */
int	signal_configure(int signal_mask, int mode_mask, int context_mask)
{
	int	result;

	(void)context_mask;
	result = 0;
	if (signal_mask & SIG_MASK_INT)
	{
		if (mode_mask & MODE_MASK_DEFAULT)
			result |= signal_set_disposition(SIGINT, SIG_DFL);
		else if (mode_mask & MODE_MASK_IGNORE)
			result |= signal_set_disposition(SIGINT, SIG_IGN);
		else if (mode_mask & MODE_MASK_INTERACTIVE)
			result |= signal_install(SIGINT, sigint_interactive_handler);
		else if (mode_mask & MODE_MASK_MONITOR)
			result |= signal_install(SIGINT, sigint_monitor_handler);
	}
	if (signal_mask & SIG_MASK_QUIT)
	{
		if (mode_mask & MODE_MASK_DEFAULT)
			result |= signal_set_disposition(SIGQUIT, SIG_DFL);
		else if (mode_mask & MODE_MASK_IGNORE)
			result |= signal_set_disposition(SIGQUIT, SIG_IGN);
		else if (mode_mask & MODE_MASK_MONITOR)
			result |= signal_install(SIGQUIT, sigquit_monitor_handler);
	}
	return (result);
}

/**
 * @brief Install a custom signal handler for a given signal.
 *
 * Sets up a handler function for the specified signal using sigaction with SA_SIGINFO.
 *
 * @param signo   Signal number to configure (e.g., SIGINT, SIGQUIT).
 * @param handler Pointer to handler function (int, siginfo_t *, void *).
 *
 * @return int   0 on success; non-zero if sigaction fails.
 */
int	signal_install(int signo, void (*handler)(int, siginfo_t *, void *))
{
	struct sigaction	sa;

	memset(&sa, 0, sizeof(sa));
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = handler;
	return (sigaction(signo, &sa, NULL));
}

/**
 * @brief Set the disposition for a signal (default or ignore).
 *
 * Configures the signal handler for the given signal to SIG_DFL or SIG_IGN.
 *
 * @param signo        Signal number to configure.
 * @param disposition  Handler disposition (SIG_DFL or SIG_IGN).
 *
 * @return int         0 on success; non-zero if sigaction fails.
 */
int	signal_set_disposition(int signo, void (*disposition)(int))
{
	struct sigaction	sa;

	memset(&sa, 0, sizeof(sa));
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = disposition;
	return (sigaction(signo, &sa, NULL));
}
