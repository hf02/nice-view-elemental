
> [!important]
> I've moved the repository over here: https://github.com/hf02/nice-view-turtle

# **modification of** nice!view Elemental

![image](https://github.com/user-attachments/assets/faa4130b-564e-4f3d-8f19-0bfeca774c4c)

## why

- i found the text a bit hard to read, especially with it being narrow. so i got rid of all its class and style (`:(`), and now it's just some plain text. the font is Pixel Operator
    - there's also a lil indicator for when you divert into another layer. [the values are currently hardcoded in this switch statement](https://github.com/hf02/nice-view-elemental/blob/7e8f00b06e12f21e6d918a55c167d5fb03c05247/boards/shields/nice_view_elemental/src/central/render.c#L101).
- the nice!view already has bezels, so we can use those as padding by getting rid of any padding on the screen panel itself. and now we got more screen space!
- i dont like animations, so we got the google turtle instead

## todo
- make the top bar look more consistent with everything else
- put something on the left side on the central board. an image? wpm? i dunno!
