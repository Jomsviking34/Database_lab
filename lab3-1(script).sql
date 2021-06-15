CREATE TABLE institutes (
    institute_id INT NOT NULL AUTO_INCREMENT,
    name VARCHAR(60) NOT NULL,
    PRIMARY KEY (institute_id)
);
create table tasks(task_id int not null auto_increment,primary key(task_id));
alter table tasks add column subject varchar(100) NOT NULL;
alter table tasks add column problem_statement_date date NOT NULL;
alter table tasks add column problem_solution_date date;
alter table tasks add column status varchar(50);
alter table tasks add column institute_id_fk int not null;
alter table tasks add constraint institute_id_fk
foreign key (institute_id_fk)
references institutes(institute_id);
create table customers(customer_id int not null auto_increment,name varchar(60) not null,organization_type varchar(60),primary key(customer_id));
alter table tasks add column customer_id_fk int not null; 
alter table tasks add constraint customer_id_fk
foreign key (customer_id_fk)
references customers(customer_id);
create table research_divisions(research_division_id int not null auto_increment,institute_id_fk int not null, foreign key(institute_id_fk) references institutes(institute_id),primary key(research_division_id));
create table research_division_staff(person_id int not null auto_increment,
research_division_id_fk int not null,
foreign key(research_division_id_fk) references research_divisions(research_division_id),
full_name varchar(80) not null,
experience int not null,
primary key(person_id));
alter table research_division_staff add column position int not null;
create table technical_divisions(technical_division_id int not null auto_increment,institute_id_fk int not null, foreign key(institute_id_fk) references institutes(institute_id),primary key(technical_division_id));
create table technical_division_staff(person_id int not null auto_increment,
technical_division_id_fk int not null,
foreign key(technical_division_id_fk) references technical_divisions(technical_division_id),
full_name varchar(80) not null,
experience int not null,
position int not null ,
primary key(person_id));
create table positions(position_id int not null auto_increment,name varchar(60),salary int not null,primary key(position_id));
alter table research_division_staff add constraint position_fk
foreign key (position)
references positions(position_id);
alter table technical_dtechnical_division_staffivision_staff add constraint position_fk1
foreign key (position)
references positions(position_id);