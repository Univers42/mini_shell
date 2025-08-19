/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 12:29:26 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/19 12:32:35 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

t_parse_err ms_parse_line(const char *line, t_cmdline *out);
void        ms_cmdline_free(t_cmdline *cmd);

#endif