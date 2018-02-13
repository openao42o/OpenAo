select c.MonsterName Summoner, b.MonsterName Summoned, a.*, (case when a.charging <> 0 then 1 else 0 end) HasCharging
from ti_iteminfo a
inner join ti_monster b
on a.ParameterValue1 = b.UniqueNumber
inner join ti_monster c
on a.ItemNum in (
c.MonsterItem01,
c.MonsterItem02,
c.MonsterItem03,
c.MonsterItem04,
c.MonsterItem05,
c.MonsterItem06,
c.MonsterItem07,
c.MonsterItem08,
c.MonsterItem09,
c.MonsterItem10,
c.MonsterItem11,
c.MonsterItem12,
c.MonsterItem13,
c.MonsterItem14,
c.MonsterItem15)
where a.DestParameter1 = 52
order by HasCHarging asc