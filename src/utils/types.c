/*
 * types.c
 * Copyright (C) Carl Philipp Klemm 2021 <carl@uvos.xyz>
 * 
 * types.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * types.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "types.h"
#include "sphone-log.h"
#include "comm.h"

const char *sphone_get_state_string(sphone_call_state_t state)
{
	switch(state) {
		case SPHONE_CALL_ACTIVE:
			return "Active";
		case SPHONE_CALL_HELD:
			return "Held";
		case SPHONE_CALL_DIALING:
			return "Dialing";
		case SPHONE_CALL_ALERTING:
			return "Alerting";
		case SPHONE_CALL_INCOMING:
			return "Incoming";
		case SPHONE_CALL_WATING:
			return "Wating";
		case SPHONE_CALL_DISCONNECTED:
			return "Disconnected";
		default:
			return "Unkown";
	}
}

void contact_free(Contact *contact)
{
	if(!contact)
		return;
	g_free(contact->name);
	if(contact->photo)
		g_object_unref(G_OBJECT(contact->photo));
	g_free(contact->line_identifier);
	g_free(contact);
}

Contact *contact_copy(const Contact *contact)
{
	if(!contact)
		return NULL;
	Contact *new_contact = g_malloc0(sizeof(*new_contact));
	new_contact->name = g_strdup(contact->name);
	new_contact->line_identifier = g_strdup(contact->line_identifier);
	new_contact->backend = contact->backend;
	if(contact->photo) {
		new_contact->photo = contact->photo;
		g_object_ref(G_OBJECT(contact->photo));
	}
	return new_contact;
}

bool contact_cmp(const Contact *a, const Contact *b)
{
	if(!a || !b)
		return !a && !b;
	return a->backend == b->backend && g_strcmp0(a->line_identifier, b->line_identifier) == 0;
}

void contact_print(const Contact *contact, const char *module_name)
{
	if(contact) {
		CommBackend *backend = sphone_comm_get_backend(contact->backend);
		sphone_log(LL_DEBUG, "%s%sContact %s %s from %s",
				   module_name ?: "", module_name ? ": " : "",
				   contact->name ?: "<unknown>",
				   contact->line_identifier ?: "",
				   backend ? backend->name : "unknown");
	} else {
		sphone_log(LL_DEBUG, "%s%sContact: NULL", module_name ?: "", module_name ? ": " : "" );
	}
}

const Contact *contact_from_message(const MessageProperties *msg)
{
	static Contact contact = {0};
	contact.line_identifier = msg->line_identifier;
	contact.backend = msg->backend;
	return &contact;
}

const Contact *contact_from_call(const CallProperties *call)
{
	static Contact contact = {0};
	contact.line_identifier = call->line_identifier;
	contact.backend = call->backend;
	return &contact;
}

void call_properties_free(CallProperties *properties)
{
	if(!properties)
		return;
	contact_free(properties->contact);
	g_free(properties->line_identifier);
	g_free(properties->backend_data);
	g_free(properties);
}

bool call_properties_comp(const CallProperties *a, const CallProperties *b)
{
	return a->backend == b->backend && g_strcmp0(a->line_identifier, b->line_identifier) == 0;
}

CallProperties *call_properties_copy(const CallProperties *properties)
{
	if(!properties)
		return NULL;
	CallProperties *new_props = g_malloc0(sizeof(*new_props));
	new_props->contact = contact_copy(properties->contact);
	new_props->line_identifier = g_strdup(properties->line_identifier);
	new_props->backend_data = g_strdup(properties->backend_data);
	new_props->start_time = properties->start_time;
	new_props->end_time = properties->end_time;
	new_props->awnserd = properties->awnserd;
	new_props->backend = properties->backend;
	new_props->state = properties->state;
	new_props->needs_route = properties->needs_route;
	new_props->outbound = properties->outbound;
	return new_props;
}

void call_properties_print(const CallProperties *call, const char *module_name)
{
	if(!call) {
		sphone_log(LL_DEBUG, "%s%sCall: NULL", module_name ?: "", module_name ? ": " : "" );
		return;
	}
	
	CommBackend *backend = sphone_comm_get_backend(call->backend);
	if(!backend) {
		sphone_log(LL_WARN, "%s%sGot invalid backend id %d", module_name ?: "", module_name ? ": " : "", call->backend);
		return;
	}

	sphone_log(LL_DEBUG, "%s%sCall: %s from %s", module_name ?: "", module_name ? ": " : "",
		       call->line_identifier, backend->name);
	contact_print(call->contact, module_name);
	sphone_log(LL_DEBUG, "%s%sstart_time: %ld awnserd: %d state: %s outbound: %d ", module_name ?: "", module_name ? ": " : "",
		       call->start_time, call->awnserd, sphone_get_state_string(call->state), call->outbound);
}

MessageProperties *message_properties_copy(const MessageProperties *properties)
{
	if(!properties)
		return NULL;
	MessageProperties *new_props = g_malloc0(sizeof(*new_props));
	new_props->contact = contact_copy(properties->contact);
	new_props->text = g_strdup(properties->text);
	new_props->line_identifier = g_strdup(properties->line_identifier);
	new_props->technology = g_strdup(properties->technology);
	new_props->backend_data = g_strdup(properties->backend_data);
	new_props->time = properties->time;
	new_props->backend = properties->backend;
	new_props->outbound = properties->outbound;
	return new_props;
}

void message_properties_free(MessageProperties *properties)
{
	contact_free(properties->contact);
	g_free(properties->line_identifier);
	g_free(properties->technology);
	g_free(properties->text);
	g_free(properties->backend_data);
	g_free(properties);
}

void message_properties_print(const MessageProperties *msg, const char *module_name)
{
	if(!msg) {
		sphone_log(LL_DEBUG, "%s%sMessage: NULL", module_name ?: "", module_name ? ": " : "" );
		return;
	}
	
	CommBackend *backend = sphone_comm_get_backend(msg->backend);
	if(!backend) {
		sphone_log(LL_WARN, "%s%sGot invalid backend id %d", module_name ?: "", module_name ? ": " : "", msg->backend);
		return;
	}

	sphone_log(LL_DEBUG, "%s%sMessage: %s from %s", module_name ?: "", module_name ? ": " : "",
		       msg->line_identifier, backend->name);
	contact_print(msg->contact, module_name);
	sphone_log(LL_DEBUG, "%s%stime: %ld text: %s outbound: %d ", module_name ?: "", module_name ? ": " : "",
		       msg->time, msg->text, msg->outbound);
}

void notification_free(Notification *notification)
{
	g_free(notification->title);
	g_free(notification->text);
	g_free(notification);
}
