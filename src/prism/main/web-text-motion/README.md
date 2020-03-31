# 使用文档

触发事件名称: textMotion

## 参数说明

|  字段   | 类型  |描述   | 默认值  |
|  ---  | ---  |---  | ---  |
| type  | String |必传|null|
| isOnce  | Boolean |是否为循环动画，false则为循环动画|false|
| background  | String | RGBA，或者16进制的一个数值  | null|
| content  | String | 主要内容信息| null |
| subContent  | String | 子内容 ||
| fontFamily  | String| 字体名称 | null|
| fontStyle  | String| 字体样式，斜体，粗细等，多个用空格隔开，比如粗斜体为“Bold Italic”| null|
| fontColor  | String | 字体颜色 | null |
| textLineSize  | String | 轮廓线厚度 | null |
| textLineColor  | String | 轮廓线颜色| null |
| fontSize  | String |主要字体大小，需要带上单位 | 无 |
| align  | String |横向对齐，left ，center，right| 无 |
| animateRate  | String |动画时间，1为正常，小于1为等比加速，大于1为等比减速|1|
| textTransform  | String |none,lowercase,uppercase|none|

- type

所有支持的type类型： 

```

[
  'Title_A1', 'Title_A2', 'Title_A3', 'Title_A4', 'Title_B1',
  'Title_B2', 'Title_B3', 'Title_B4', 'Element_2', 'Element_1',
  'Caption_3', 'Caption_2', 'Caption_1', 'Lower_Third_1', 'Lower_Third_2',
  'Social_1', 'Social_2', 'Social_3', 'Social_4', 'Social_5', 'Social_6', 'Social_7'
]

```
    

## 备注

type为Title_B3时，每个文字组合由','号隔开，最终展示最后一个文字 

type为Caption_2时，'\\\l'为高亮的一行

## 示例

```
window.dispatchEvent(new CustomEvent('textMotion', {
  detail: {
    "animateRate":"1",
    "background":"#450000",
    "content":"Content",
    "subContent":"SubContent",
    "fontColor":"#be1800",
    "fontSize":"78px",
    "fontStyle":"Regular Italic",
    "align":"left",
    "textLineColor":"#d62c00",
    "textLineSize":"8px",
    "type":"Title_A1"
}
}))
```
