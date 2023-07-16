select * 
from dexcom d 
left join hr on (hr.patient = d.patient and DATE_FORMAT(hr.`timestamp`, '%Y-%m-%d %H:%i') = DATE_FORMAT(d.`timestamp`, '%Y-%m-%d %H:%i')) ;

select * 
from dexcom d 
where d.patient = 1 
and DATE_FORMAT(d.`timestamp`, '%Y-%m-%d %H:%i') = DATE_FORMAT(hr.`timestamp` , '%Y-%m-%d %H:%i');

select timestamp('2020-02-15 18:08:30.000', '00:00:30.000');

SELECT 
	d.patient,
    DATE_FORMAT(d.`timestamp`, '%Y-%m-%d %H:%i') AS minute, 
    AVG(d.glucose) AS average_value
from dexcom d 
GROUP BY 
    d.patient, minute;
   
create or replace view view_dexcom as
select
	d.userid as uid,
	STR_TO_DATE(DATE_FORMAT(d.`Timestamp (YYYY-MM-DDThh:mm:ss)`, '%Y-%m-%d %H:%i'),'%Y-%m-%d %H:%i') AS timeofmeasurement,
	d.`Glucose Value (mg/dL)` as glucose
from dexcom d;

create or replace view view_hr as
SELECT 
	hr.userid as uid,
	STR_TO_DATE(DATE_FORMAT(hr.`datetime`, '%Y-%m-%d %H:%i'),'%Y-%m-%d %H:%i') AS timeofmeasurement,
    AVG(hr.` hr`) AS heartrate
from hr
GROUP BY 
    uid, timeofmeasurement;
-- 
create or replace view view_temperature as
SELECT 
	temp.userid as uid,
	STR_TO_DATE(DATE_FORMAT(temp.`datetime`, '%Y-%m-%d %H:%i'),'%Y-%m-%d %H:%i') AS timeofmeasurement,
    AVG(temp.` temp`) AS temperature
from temp 
GROUP BY 
    uid, timeofmeasurement;
-- 
create or replace view view_ibi as
SELECT 
	ibi.userid as uid,
	STR_TO_DATE(DATE_FORMAT(ibi.`datetime`, '%Y-%m-%d %H:%i'),'%Y-%m-%d %H:%i') AS timeofmeasurement,
    AVG(ibi.` ibi`) AS ibi
from ibi
GROUP BY 
    uid, timeofmeasurement;
--   
select count(hr.userid) from hr; -- 9.2M
select count(vh.uid) from view_hr vh ; -- 155K 

select count(t.userid) from temp t; -- 37M
select count(vt.uid) from view_temperature vt; -- 155K

create or replace view view_data as
select vd.uid, vd.timeofmeasurement, vh.heartrate, vi.ibi, vt.temperature, vd.glucose 
from view_dexcom vd
inner join view_hr vh on vh.uid = vd.uid and vh.timeofmeasurement = vd.timeofmeasurement
inner join view_ibi vi on vi.uid = vd.uid and vi.timeofmeasurement = vd.timeofmeasurement
inner join view_temperature vt on vt.uid = vd.uid and vt.timeofmeasurement = vd.timeofmeasurement;

-- to validate averages   
-- SELECT 
-- 	hr.patient,
--     hr.`timestamp`,
--     hr.value
-- from hr
-- where hr.patient = 3
-- and DATE_FORMAT(hr.`timestamp`, '%Y-%m-%d %H:%i') = '2020-03-01 11:34'
