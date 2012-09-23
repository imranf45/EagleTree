/* Copyright 2009, 2010 Brendan Tauras */

/* ssd_event.cpp is part of FlashSim. */

/* FlashSim is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version. */

/* FlashSim is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details. */

/* You should have received a copy of the GNU General Public License
 * along with FlashSim.  If not, see <http://www.gnu.org/licenses/>. */

/****************************************************************************/

/* Event class
 * Brendan Tauras 2010-07-16
 *
 * Class to manage I/O requests as events for the SSD.  It was designed to keep
 * track of an I/O request by storing its type, addressing, and timing.  The
 * SSD class creates an instance for each I/O request it receives.
 */

#include <assert.h>
#include <stdio.h>
#include "ssd.h"

using namespace ssd;

uint Event::id_generator = 0;
uint Event::application_io_id_generator = 0;

/* see "enum event_type" in ssd.h for details on event types */
Event::Event(enum event_type type, ulong logical_address, uint size, double start_time):
	start_time(start_time),
	time_taken(0.0),
	bus_wait_time(0.0),
	os_wait_time(0.0),
	type(type),
	logical_address(logical_address),
	size(size),
	payload(NULL),
	//next(NULL),
	noop(false),
	id(id_generator++),
	application_io_id(application_io_id_generator++),
	garbage_collection_op(false),
	mapping_op(false),
	original_application_io(false),
	age_class(0),
	tag(-1)
{
	assert(start_time >= 0.0);
	if (logical_address > NUMBER_OF_ADDRESSABLE_BLOCKS() * BLOCK_SIZE) {
		printf("invalid logical address, too big  %d   %d\n", logical_address, NUMBER_OF_ADDRESSABLE_BLOCKS() * BLOCK_SIZE);
		assert(false);
	}
}

Event::Event(Event& event) :
	start_time(event.start_time),
	time_taken(event.time_taken),
	bus_wait_time(event.bus_wait_time),
	type(event.type),
	logical_address(event.logical_address),
	size(event.size),
	payload(NULL),
	//next(NULL),
	noop(event.noop),
	id(id_generator++),
	application_io_id(event.application_io_id),
	garbage_collection_op(event.garbage_collection_op),
	mapping_op(event.mapping_op),
	original_application_io(event.original_application_io),
	age_class(event.age_class),
	tag(event.tag)
{}

Event::Event() : type(NOT_VALID) {}

// Outcommented - all setters and getters inlined
/*
inline ulong Event::get_logical_address(void) const
{
	return logical_address;
}

uint Event::get_id(void) const
{
	return id;
}

int Event::get_tag() const {
	return tag;
}

void Event::set_tag(int new_tag) {
	tag = new_tag;
}

const Address &Event::get_address(void) const
{
	return address;
}

const Address &Event::get_merge_address(void) const
{
	return merge_address;
}

const Address &Event::get_log_address(void) const
{
	return log_address;
}

const Address &Event::get_replace_address(void) const
{
	return replace_address;
}

void Event::set_log_address(const Address &address)
{
	log_address = address;
}

uint Event::get_size(void) const
{
	return size;
}

enum event_type Event::get_event_type(void) const
{
	return type;
}

void Event::set_event_type(const enum event_type &type)
{
	this->type = type;
}

{
double Event::get_start_time(void) const
	assert(start_time >= 0.0);
	return start_time;
}

double Event::get_time_taken(void) const
{
	assert(time_taken >= 0.0);
	return time_taken;
}

double Event::get_current_time(void) const {
	return start_time + time_taken;
}

double Event::get_ssd_submission_time() const {
	return start_time + os_wait_time;
}

uint Event::get_application_io_id(void) const {
	return application_io_id;
}

double Event::get_os_wait_time(void) const {
	return os_wait_time;
}

double Event::get_bus_wait_time(void) const
{
	assert(bus_wait_time >= 0.0);
	return bus_wait_time;
}

bool Event::get_noop(void) const
{
	return noop;
}

void Event::set_payload(void *payload)
{
	this->payload = payload;
}

void *Event::get_payload(void) const
{
	return payload;
}

void Event::set_address(const Address &address)
{
	if (type == WRITE || type == READ || type == READ_COMMAND || type == READ_TRANSFER) {
		assert(address.valid == PAGE);
	}
	this -> address = address;
}

void Event::set_merge_address(const Address &address)
{
	merge_address = address;
}

void Event::set_replace_address(const Address &address)
{
	replace_address = address;
}

void Event::set_noop(bool value)
{
	noop = value;
}

bool Event::is_original_application_io(void) const {
	return original_application_io;
}

void Event::set_original_application_io(bool val) {
	original_application_io = val;
}

void Event::set_application_io_id(uint value) {
	application_io_id = value;
}

void Event::set_garbage_collection_op(bool value) {
	garbage_collection_op = value;
}

void Event::set_mapping_op(bool value) {
	mapping_op = value;
}

void Event::set_age_class(int value) {
	age_class = value;
}

int Event::get_age_class() const {
	return age_class;
}

bool Event::is_garbage_collection_op() const {
	return garbage_collection_op;
}

bool Event::is_mapping_op() const {
	return mapping_op;
}

void Event::set_start_time(double value) {
	assert(value > 0);
	start_time = value;
}

double Event::incr_bus_wait_time(double time_incr)
{
	if(time_incr > 0.0)
		bus_wait_time += time_incr;
	return bus_wait_time;
}

double Event::incr_time_taken(double time_incr)
{
  	if(time_incr > 0.0)
		time_taken += time_incr;
	return time_taken;
}

double Event::incr_os_wait_time(double time_incr)
{
  	if(time_incr > 0.0)
		os_wait_time += time_incr;
	return os_wait_time;
}
*/
void Event::print(FILE *stream) const
{
	if (type == NOT_VALID)
		fprintf(stream, "<NOT VALID> ");
	else if(type == READ)
		fprintf(stream, "R ");
	else if(type == READ_COMMAND)
		fprintf(stream, "C ");
	else if(type == READ_TRANSFER)
		fprintf(stream, "T ");
	else if(type == WRITE)
		fprintf(stream, "W ");
	else if(type == ERASE)
		fprintf(stream, "E ");
	else if(type == MERGE)
		fprintf(stream, "M ");
	else if (type == TRIM)
		fprintf(stream, "D ");
	else if (type == GARBAGE_COLLECTION)
		fprintf(stream, "GC ");
	else if (type == COPY_BACK)
		fprintf(stream, "CB ");
	else
		fprintf(stream, "Unknown event type: ");

	fprintf(stream, "%d\t", logical_address);
	address.print(stream);
	//if(type == MERGE)
		//merge_address.print(stream);
	//if(type == WRITE || type == TRIM || type == COPY_BACK)
		//replace_address.print(stream);
	//fprintf(stream, " Time[%d, %d, %d, %d]", (int)start_time, (int)get_ssd_submission_time(), (int)get_current_time(), (int)bus_wait_time);
	fprintf(stream, "\tTime[%d, %d, %d, %d, %d]", (int)start_time, (int)os_wait_time, (int)bus_wait_time,(int) bus_wait_time + (int)os_wait_time, (int) get_current_time());
	fprintf(stream, " ID: %d ", id);
	fprintf(stream, " appID: %d", application_io_id);

	if (garbage_collection_op) {
		fprintf(stream, " GC");
	} else if (mapping_op)  {
		fprintf(stream, " MAPPING");
	}
	if (original_application_io) {
		fprintf(stream, " APP");
	}
	if (type == GARBAGE_COLLECTION) {
		fprintf(stream, " age class: %d", age_class);
	}
	fprintf(stream, "\n");
}

void Event::reset_id_generators() {
	Event::id_generator = 0;
	Event::application_io_id_generator = 0;
}

#if 0
/* may be useful for further integration with DiskSim */

/* caution: copies pointers from rhs */
ioreq_event &Event::operator= (const ioreq_event &rhs)
{
	assert(&rhs != NULL);
	if((const ioreq_event *) &rhs == (const ioreq_event *) &(this -> ioreq))
		return *(this -> ioreq);
	ioreq -> time = rhs.time;
	ioreq -> type = rhs.type;
	ioreq -> next = rhs.next;
	ioreq -> prev = rhs.prev;
	ioreq -> bcount = rhs.bcount;
	ioreq -> blkno = rhs.blkno;
	ioreq -> flags = rhs.flags;
	ioreq -> busno = rhs.busno;
	ioreq -> slotno = rhs.slotno;
	ioreq -> devno = rhs.devno;
	ioreq -> opid = rhs.opid;
	ioreq -> buf = rhs.buf;
	ioreq -> cause = rhs.cause;
	ioreq -> tempint1 = rhs.tempint1;
	ioreq -> tempint2 = rhs.tempint2;
	ioreq -> tempptr1 = rhs.tempptr1;
	ioreq -> tempptr2 = rhs.tempptr2;
	ioreq -> mems_sled = rhs.mems_sled;
	ioreq -> mems_reqinfo = rhs.mems_reqinfo;
	ioreq -> start_time = rhs.start_time;
	ioreq -> batchno = rhs.batchno;
	ioreq -> batch_complete = rhs.batch_complete;
	ioreq -> batch_size = rhs.batch_size;
	ioreq -> batch_next = rhs.batch_next;
	ioreq -> batch_prev = rhs.batch_prev;
	return *ioreq;
}
#endif
