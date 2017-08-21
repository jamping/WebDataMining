--delete the tables;
drop table wdm_picture;
drop table wdm_topic;
drop table wdm_entry;
drop table wdm_rule_tree;
drop table wdm_rule;
drop table wdm_schedule;
drop table wdm_column;
drop table wdm_operator_unit;
drop table wdm_operator;
drop table wdm_table;
drop table wdm_database;
drop table wdm_server;
drop table wdm_k_type;
drop table wdm_user;
drop table wdm_log;

go
--delete the table data

delete from  wdm_picture
delete from  wdm_topic
delete from  wdm_entry
delete from  wdm_rule_tree
delete from  wdm_rule
delete from  wdm_schedule
delete from  wdm_column
delete from  wdm_operator_unit
delete from  wdm_operator
delete from  wdm_table
delete from  wdm_database
delete from wdm_server
delete from  wdm_k_type
delete from  wdm_user
delete from  wdm_log

go
--------------------------------------------------------------------------------------------
--wdm_user
create table wdm_user
(
user_name          varchar(20) primary key,
user_passwd        varchar(255),-----Added at 2016.11.10
user_level         int,
user_desc          varchar(60),
create_time        datetime
)
go

--wdm_log
create table wdm_log
(
log_id            varchar(40) primary key,
log_time          datetime,
log_source        varchar(40),
log_type          int,
log_message       varchar(300),
log_owner         varchar(40)
)
go

--wdm_server
create table wdm_server
(
server_id     varchar(40) primary key,
server_name   varchar(40),
user_name     varchar(40),
user_passwd   varchar(255),-----Added at 2016.11.10
resource_dir  varchar(255),
server_desc   varchar(300),
database_type int,
resource_path varchar(255) ,  -----Added at 2014.5.7
resource_thumb int, -----Added at 2016.10.24
resource_thumb_name varchar(20),-----Added at 2016.10.24
server_port int,
server_code varchar(20),
file_send_type int,
file_server_name varchar(255),
file_server_user_name varchar(40),
file_server_password varchar(40),
file_server_port  int
)
go

--wdm_database
create table wdm_database
(
database_id       varchar(40) primary key,
server_id         varchar(40),
database_name     varchar(40),
constraint wdm_database_FK foreign key(server_id) references wdm_server
)
go

--wdm_table
create table wdm_table
(
table_id                  varchar(40) primary key,
database_id          varchar(40),
table_name           varchar(40),   
table_type             int,            -----Added at 2007.07.06
process_order     int,           -------added at 2014,5,19
publish_repeat      int,              ---added at 2016.11.2
constraint wdm_table_FK foreign key(database_id) references wdm_database
)
go

--wdm_operator
create table wdm_operator
(
operator_id       varchar(40) primary key,
table_id          varchar(40),
operator_name     varchar(20),
constraint wdm_operator_FK foreign key(table_id) references wdm_table
)
go

--wdm_operator_unit
create table wdm_operator_unit
(
unit_id               varchar(40) primary key,
operator_id      varchar(40),
unit_name        varchar(40),
unit_length        int,
unit_value         varchar(255),----Modifyed at 2014.5.26
unit_type          varchar(40),----Added at 2014.5.26
--topic_unit         int,    Modifyed at 2016.10.28
is_key               int,
is_cache             int,  -----added at 2016.10.28
unit_default         varchar(255),
unit_key             varchar(40),
unit_isnull          varchar(5),
unit_combos      varchar(255),  ----Added at 2014.5.26
--unit_separator varchar(10),----Added at 2014.5.26
process_order     int,           -------added at 2014,5,19
constraint wdm_operator_unit_FK foreign key(operator_id) references wdm_operator
)
go

--wdm_column 
create table wdm_column
(
column_id    varchar(40) primary key,
database_id  varchar(40),
parent_id    varchar(40),
column_name  varchar(20) null
)
go


--wdm_rule
create table wdm_rule
(
rule_id		     varchar(40) primary key, 
rule_name            varchar(20),
rule_encoding        int    ----added at 2007.12.22
)
go

--wdm_rule_tree
create table wdm_rule_tree
(
rule_tree_id         varchar(40) primary key,
rule_id              varchar(40), 
parent_id            varchar(40), 
rule_tree_name       varchar(20),
match_rule           varchar(300),
process_type         int,
save_type            int,
constraint wdm_rule_tree_FK foreign key(rule_id) references wdm_rule
)
go

--wdm_schedule
create table wdm_schedule
(
schedule_id              varchar(40) primary key,
object_id                varchar(40),
object_type              int,
schedule_name            varchar(60),
enabled                  int,
freq_type                int,
freq_interval            int,
freq_subday_type         int,
freq_subday_interval     int,
freq_relative_interval   int,
freq_recurrence_factor   int,
active_start_date        int,
active_end_date          int,
active_start_time        int,
active_end_time          int,
next_run_date            int,
next_run_time            int,
created_date             datetime           
)
go

--wdm_entry
create table wdm_entry
(
entry_id  		varchar(40) primary key,
column_id  		varchar(40), 
server_column_id        varchar(40),
--server_channel_id       varchar(40),    -----Modified at 2007.12.14
rule_id                 varchar(40), 
entry_name 		varchar(60),
website_name            varchar(20),
url     		varchar(300),
match_rule              varchar(300),
topic_max		int,
try_times 		int,
is_auto_publish		int,-----added at 2007.12.26
last_download_time    	datetime,
constraint wdm_entry_FK1 foreign key (column_id) references wdm_column
)
go

--wdm_topic
create table wdm_topic
(
topic_id 		varchar(40) primary key,
server_auto_id  varchar(40), ---------modified at 2016.10.31
entry_id		varchar(40),
url                     varchar(300),
title 	        	varchar(100) not null,
main_title              varchar(100),-----Added at 2007.12.12
head_title		varchar(300),
sub_title		varchar(300),
source			varchar(100),
time                    datetime,
author                  varchar(50),
keyword			varchar(100),-----Added at 2007.12.12
subject			varchar(100),
content			text,
is_primary              int,         -----Added at 2007.12.12
is_elite                int,         -----Added at 2007.12.12
is_ontop                int,         -----Added at 2007.12.12
is_publish		int,
is_sendto		int,
is_save_image_flag      int,         -----added at 2007.12.26
--primary_number          int,         -----modified at 2017.02.03
weight                           int,         ------added at 2017.02.03
server_column_ids       text,        -----Added at 2007.12.12
download_date           datetime,
thumb           varchar(200),        -----Added at 2016.10.24
old_title       varchar(100),        -----Added at 2016.10.24
constraint wdm_topic_FK foreign key (entry_id) references wdm_entry
)
go

--wdm_picture
create table wdm_picture
(
picture_id             varchar(40) primary key,
topic_id               varchar(40),
url                    varchar(300),
old_url                varchar(300),  --added at 2008.01.22
explain                text,
start_pos              int,    --added at 2007.12.20
end_pos                int,    --added at 2007.12.20
server_auto_id         varchar(40), -----modified at 2016.10.31
picture_size          int,---------added at 2014.5.19
is_thumb           int,----------added at 2016.10.24
constraint wdm_picture_FK foreign key(topic_id) references wdm_topic
)
go

--wdm_k_type
create table wdm_k_type
(
type_id               varchar(40) primary key,
parent_id           varchar(40),
type_name        varchar(40),
)
go