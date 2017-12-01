create table connection(
id int primary key,
name text not null,
url text not null,
token text not null,
batch_presence_aware boolean not null);

create table user(
id int primary key,
code text not null,
name text not null);

create table team(
id int primary key,
code text not null,
name text not null,
domain text not null);
