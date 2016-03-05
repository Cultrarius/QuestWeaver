# Template Schema

Template files used by QuestWeaver use a **JSON notation**. 
This document shows how the template files for the quests and stories are structured.

## Quest Template Files

`<Document Content>` =
```json
{
  "parent": <String>,
  "key": <String>,
  "mandatory": <StringArray>,
  "optional": <StringArray>,
  "titles": [
    <String>
  ],
  "descriptions": [
    <Description>
  ],
  "objectives": [
    <String>
  ]
}
```

`<String>` = any valid JSON string (e.g. "Foo")

`<Description>`=
```json
{
  "conditions": [
    <String>
  ],
  "text": <String>
}
```

## Story Template Files

`<Document Content>` =
```json
[
  <Story>
]
```

`<String>` = any valid JSON string (e.g. "Foo")

`<Story>`=
```json
{
  "key": <String>,
  "required": [
    <String>
  ],
  "lines": [
    <Line> or <String>
  ]
}
```

`<Line>`=
```json
{
  "pre": <String>,
  "nuggets": [
    <String>
  ],
  "post": <String>
}
```

## Story Nugget Files

`<Document Content>` =
```json
[
  <Nugget>
]
```

`<String>` = any valid JSON string (e.g. "Foo")

`<Nugget>`=
```json
{
  "key": <String>,
  "requiredTypes": [
    <String>
  ],
  "texts": [
    <String>
  ]
}
```