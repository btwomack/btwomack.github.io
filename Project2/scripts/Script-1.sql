create table employee_info(

employee_id serial primary key,
employee_first_name varchar ,
employee_last_name varchar ,
employee_username varchar ,
employee_password varchar

);

create table manager_info(

manager_id serial primary key,
manager_first_name varchar ,
manager_last_name varchar ,
manager_username varchar ,
manage_password varchar

);


create table employee_transactions(

trans_id serial primary key,
trans_title varchar ,
trans_description varchar,
trans_amount float ,
approval boolean ,
approval_description varchar,
employee_id int references employee_info(employee_id)

);