
select * from ti_rareiteminfo

select a.ReqItemKind, b.ReqItemKind, a.Name, b.Name, a.CodeNum, b.CodeNum, a.[DesParameter1]
      ,a.[ParameterValue1]
      ,a.[DesParameter2]
      ,a.[ParameterValue2]
      ,a.[DesParameter3]
      ,a.[ParameterValue3]
      ,a.[DesParameter4]
      ,a.[ParameterValue4]
	  ,b.[DesParameter1]
      ,b.[ParameterValue1]
      ,b.[DesParameter2]
      ,b.[ParameterValue2]
      ,b.[DesParameter3]
      ,b.[ParameterValue3]
      ,b.[DesParameter4]
      ,b.[ParameterValue4]

from ti_rareiteminfo a
left join ti_rareiteminfo b
on a.Name like b.Name and a.CodeNum <> b.CodeNum
where a.ReqItemKind = 44 and b.ReqItemKind = 48
order by a.Name
--where a.ReqItemKind = 44 and b.ReqItemKind = 40 