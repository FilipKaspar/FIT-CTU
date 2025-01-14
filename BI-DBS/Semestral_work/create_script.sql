-- Remove conflicting tables
DROP TABLE IF EXISTS address CASCADE;
DROP TABLE IF EXISTS class CASCADE;
DROP TABLE IF EXISTS country CASCADE;
DROP TABLE IF EXISTS dorm CASCADE;
DROP TABLE IF EXISTS duration CASCADE;
DROP TABLE IF EXISTS grade CASCADE;
DROP TABLE IF EXISTS person CASCADE;
DROP TABLE IF EXISTS specialization CASCADE;
DROP TABLE IF EXISTS sport CASCADE;
DROP TABLE IF EXISTS student CASCADE;
DROP TABLE IF EXISTS teacher CASCADE;
DROP TABLE IF EXISTS person_person CASCADE;
DROP TABLE IF EXISTS sport_person CASCADE;
-- End of removing

CREATE TABLE address (
    id_address SERIAL NOT NULL,
    street VARCHAR(256) NOT NULL,
    number VARCHAR(256) NOT NULL,
    city VARCHAR(256) NOT NULL,
    zip_code VARCHAR(256)
);
ALTER TABLE address ADD CONSTRAINT pk_address PRIMARY KEY (id_address);

CREATE TABLE class (
    id_class SERIAL NOT NULL,
    id_person INTEGER NOT NULL,
    name VARCHAR(256) NOT NULL
);
ALTER TABLE class ADD CONSTRAINT pk_class PRIMARY KEY (id_class);

CREATE TABLE country (
    id_country SERIAL NOT NULL,
    name VARCHAR(256) NOT NULL
);
ALTER TABLE country ADD CONSTRAINT pk_country PRIMARY KEY (id_country);

CREATE TABLE dorm (
    id_dorm SERIAL NOT NULL,
    name VARCHAR(256) NOT NULL,
    floor VARCHAR(256),
    room VARCHAR(256)
);
ALTER TABLE dorm ADD CONSTRAINT pk_dorm PRIMARY KEY (id_dorm);

CREATE TABLE duration (
    id_duration SERIAL NOT NULL,
    id_country INTEGER NOT NULL,
    from VARCHAR(256) NOT NULL,
    to VARCHAR(256)
);
ALTER TABLE duration ADD CONSTRAINT pk_duration PRIMARY KEY (id_duration);

CREATE TABLE grade (
    id_grade SERIAL NOT NULL,
    id_class INTEGER NOT NULL,
    id_person INTEGER NOT NULL,
    grade VARCHAR(256) NOT NULL,
    semester VARCHAR(256) NOT NULL
);
ALTER TABLE grade ADD CONSTRAINT pk_grade PRIMARY KEY (id_grade);

CREATE TABLE person (
    id_person SERIAL NOT NULL,
    id_address INTEGER NOT NULL,
    id_duration INTEGER NOT NULL,
    birth_certificate_number VARCHAR(256) NOT NULL,
    first_name VARCHAR(256) NOT NULL,
    last_name VARCHAR(256) NOT NULL,
    age VARCHAR(256) NOT NULL,
    orientation VARCHAR(256)
);
ALTER TABLE person ADD CONSTRAINT pk_person PRIMARY KEY (id_person);
ALTER TABLE person ADD CONSTRAINT uc_person_birth_certificate_num UNIQUE (birth_certificate_number);

CREATE TABLE specialization (
    id_specialization SERIAL NOT NULL,
    name VARCHAR(256) NOT NULL
);
ALTER TABLE specialization ADD CONSTRAINT pk_specialization PRIMARY KEY (id_specialization);

CREATE TABLE sport (
    id_sport SERIAL NOT NULL,
    name VARCHAR(256) NOT NULL
);
ALTER TABLE sport ADD CONSTRAINT pk_sport PRIMARY KEY (id_sport);

CREATE TABLE student (
    id_person INTEGER NOT NULL,
    id_dorm INTEGER,
    id_specialization INTEGER,
    free_time VARCHAR(256) NOT NULL
);
ALTER TABLE student ADD CONSTRAINT pk_student PRIMARY KEY (id_person);

CREATE TABLE teacher (
    id_person INTEGER NOT NULL,
    survey_score VARCHAR(256) NOT NULL
);
ALTER TABLE teacher ADD CONSTRAINT pk_teacher PRIMARY KEY (id_person);

CREATE TABLE person_person (
    source_id_person INTEGER NOT NULL,
    target_id_person INTEGER NOT NULL
);
ALTER TABLE person_person ADD CONSTRAINT pk_person_person PRIMARY KEY (source_id_person, target_id_person);

CREATE TABLE sport_person (
    id_sport INTEGER NOT NULL,
    id_person INTEGER NOT NULL
);
ALTER TABLE sport_person ADD CONSTRAINT pk_sport_person PRIMARY KEY (id_sport, id_person);

ALTER TABLE class ADD CONSTRAINT fk_class_teacher FOREIGN KEY (id_person) REFERENCES teacher (id_person) ON DELETE CASCADE;

ALTER TABLE duration ADD CONSTRAINT fk_duration_country FOREIGN KEY (id_country) REFERENCES country (id_country) ON DELETE CASCADE;

ALTER TABLE grade ADD CONSTRAINT fk_grade_class FOREIGN KEY (id_class) REFERENCES class (id_class) ON DELETE CASCADE;
ALTER TABLE grade ADD CONSTRAINT fk_grade_student FOREIGN KEY (id_person) REFERENCES student (id_person) ON DELETE CASCADE;

ALTER TABLE person ADD CONSTRAINT fk_person_address FOREIGN KEY (id_address) REFERENCES address (id_address) ON DELETE CASCADE;
ALTER TABLE person ADD CONSTRAINT fk_person_duration FOREIGN KEY (id_duration) REFERENCES duration (id_duration) ON DELETE CASCADE;

ALTER TABLE student ADD CONSTRAINT fk_student_person FOREIGN KEY (id_person) REFERENCES person (id_person) ON DELETE CASCADE;
ALTER TABLE student ADD CONSTRAINT fk_student_dorm FOREIGN KEY (id_dorm) REFERENCES dorm (id_dorm) ON DELETE CASCADE;
ALTER TABLE student ADD CONSTRAINT fk_student_specialization FOREIGN KEY (id_specialization) REFERENCES specialization (id_specialization) ON DELETE CASCADE;

ALTER TABLE teacher ADD CONSTRAINT fk_teacher_person FOREIGN KEY (id_person) REFERENCES person (id_person) ON DELETE CASCADE;

ALTER TABLE person_person ADD CONSTRAINT fk_person_person_person FOREIGN KEY (source_id_person) REFERENCES person (id_person) ON DELETE CASCADE;
ALTER TABLE person_person ADD CONSTRAINT fk_person_person_person_1 FOREIGN KEY (target_id_person) REFERENCES person (id_person) ON DELETE CASCADE;

ALTER TABLE sport_person ADD CONSTRAINT fk_sport_person_sport FOREIGN KEY (id_sport) REFERENCES sport (id_sport) ON DELETE CASCADE;
ALTER TABLE sport_person ADD CONSTRAINT fk_sport_person_person FOREIGN KEY (id_person) REFERENCES person (id_person) ON DELETE CASCADE;

