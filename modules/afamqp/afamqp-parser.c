/*
 * Copyright (c) 2002-2011 BalaBit IT Ltd, Budapest, Hungary
 * Copyright (c) 2010-2011 Gergely Nagy <algernon@balabit.hu>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * As an additional exemption you are allowed to compile & link against the
 * OpenSSL libraries as published by the OpenSSL project. See the file
 * COPYING for details.
 *
 */

#include "afamqp.h"
#include "cfg-parser.h"
#include "afamqp-grammar.h"

extern int afamqp_debug;
int afamqp_parse(CfgLexer *lexer, LogDriver **instance, gpointer arg);

static CfgLexerKeyword afamqp_keywords[] = {
  { "amqp",			KW_AMQP },
  { "vhost",		KW_VHOST },
  { "host",			KW_HOST },
  { "port",			KW_PORT },
  { "exchange",			KW_EXCHANGE },
  { "exchange_type",		KW_EXCHANGE_TYPE },
  { "routing_key",		KW_ROUTING_KEY },
  { "persistent",		KW_PERSISTENT },
  { "username",			KW_USERNAME },
  { "password",			KW_PASSWORD },
  { "log_fifo_size",		KW_LOG_FIFO_SIZE  },
  { NULL }
};

CfgParser afamqp_parser =
{
#if ENABLE_DEBUG
  .debug_flag = &afamqp_debug,
#endif
  .name = "afamqp",
  .keywords = afamqp_keywords,
  .parse = (int (*)(CfgLexer *lexer, gpointer *instance, gpointer)) afamqp_parse,
  .cleanup = (void (*)(gpointer)) log_pipe_unref,
};

CFG_PARSER_IMPLEMENT_LEXER_BINDING(afamqp_, LogDriver **)
