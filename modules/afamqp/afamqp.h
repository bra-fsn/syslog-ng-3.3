/*
 * Copyright (c) 2002-2011 BalaBit IT Ltd, Budapest, Hungary
 * Copyright (c) 2010-2011 Gergely Nagy <algernon@balabit.hu>
 * Copyright (c) 2012 Nagy, Attila <bra@fsn.hu>
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

#ifndef AFAMQP_H_INCLUDED
#define AFAMQP_H_INCLUDED

#include "driver.h"
#include "value-pairs.h"

LogDriver *afamqp_dd_new(void);

void afamqp_dd_set_host(LogDriver *d, const gchar *host);
void afamqp_dd_set_port(LogDriver *d, gint port);
void afamqp_dd_set_exchange(LogDriver *d, const gchar *database);
void afamqp_dd_set_exchange_type(LogDriver *d, const gchar *exchange_type);
void afamqp_dd_set_vhost(LogDriver *d, const gchar *vhost);
void afamqp_dd_set_routing_key(LogDriver *d, const gchar *routing_key);
void afamqp_dd_set_persistent(LogDriver *d, gboolean persistent);
void afamqp_dd_set_user(LogDriver *d, const gchar *user);
void afamqp_dd_set_password(LogDriver *d, const gchar *password);
void afamqp_dd_set_value_pairs(LogDriver *d, ValuePairs *vp);

#define DIE_ON_ERROR(ret, msg) \
              if (ret < 0) { \
                      gchar *errstr = amqp_error_string(-ret); \
                      msg_error(msg, evt_tag_str("error",errstr), NULL); \
                      g_free(errstr); \
                      return FALSE; \
              }

#define DIE_ON_AMQP_ERROR(x, context) \
	switch (x.reply_type) { \
	case AMQP_RESPONSE_NORMAL: \
		break; \
\
	case AMQP_RESPONSE_NONE: \
		msg_error(context, evt_tag_str("error","missing RPC reply type"), NULL); \
		afamqp_dd_suspend(self); \
		return FALSE; \
\
	case AMQP_RESPONSE_LIBRARY_EXCEPTION: \
		msg_error(context, evt_tag_str("error",amqp_error_string(x.library_error)), NULL); \
		afamqp_dd_suspend(self); \
		return FALSE; \
\
	case AMQP_RESPONSE_SERVER_EXCEPTION: \
		switch (x.reply.id) { \
		case AMQP_CONNECTION_CLOSE_METHOD: { \
			amqp_connection_close_t *m = \
					(amqp_connection_close_t *) x.reply.decoded; \
			msg_error(context, \
				evt_tag_str("error","server connection error"), \
				evt_tag_int("code",m->reply_code), \
				evt_tag_str("text",m->reply_text.bytes), NULL); \
			free(m); \
			afamqp_dd_suspend(self); \
			return FALSE; \
		} \
		case AMQP_CHANNEL_CLOSE_METHOD: { \
			amqp_channel_close_t *m = (amqp_channel_close_t *) x.reply.decoded; \
			msg_error(context, \
					evt_tag_str("error","server channel error"), \
					evt_tag_int("code",m->reply_code), \
					evt_tag_str("text",m->reply_text.bytes), NULL); \
			free(m); \
			afamqp_dd_suspend(self); \
			return FALSE; \
		} \
		default: \
			msg_error(context, \
					evt_tag_str("error","unknown server error"), \
					evt_tag_printf("method id","0x%08X",x.reply.id), \
					NULL); \
			afamqp_dd_suspend(self); \
			return FALSE; \
		} \
		return FALSE; \
	};

#endif
