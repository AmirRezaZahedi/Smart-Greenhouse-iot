create database IOT;
use IOT;

drop table SoilMoisture;

CREATE TABLE SoilMoisture (
	type varchar(32),
    value INT,
    dateTime DATETIME,
    PRIMARY KEY (dateTime)
);

CREATE TABLE AirTemperature (
	type varchar(32),
    value INT,
    dateTime DATETIME,
    PRIMARY KEY (dateTime)
);


CREATE TABLE AirHumidity (
	type varchar(32),
    value INT,
    dateTime DATETIME,
    PRIMARY KEY (dateTime)
);

CREATE TABLE AmbientLight  (
	type varchar(32),
    value INT,
    dateTime DATETIME,
    PRIMARY KEY (dateTime)
);
