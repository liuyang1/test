from jira import JIRA

jira = JIRA('http://jira.marvell.com')

issue = jira.issue('DEBUSWI-11803')
print issue.fields.project.key             # 'JRA'
print issue.fields.issuetype.name          # 'New Feature'
print issue.fields.reporter.displayName    # 'Mike Cannon-Brookes [Atlassian]'
