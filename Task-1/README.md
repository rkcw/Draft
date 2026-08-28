# Task 1 (Password Validation and Regex)

## Focus/Aims

This task will focus on testing your skills on:

- Basic C (loop and if conditions)
- Array Manipulation
- C String

## Background

This task is mainly focused ~~(loosely based)~~ on data validation (Similar concepts will be very useful when you deal with sensors later on).

## Task Details

### Part A: Parametrised validation

### i) Password checker

We will start with some sort of warmup.

```int password_check(char* password)```

Complete the function above in `Task1.c`.

The function will process the C string `password` and return 0 or 1 based on whether the password is valid or not.

The following requirements must be fulfilled:

- The password must be at least **8** characters long, and no longer than **24** characters. (aka the password must be 8-24 characters long, inclusive)
- The password **must only include**, and **must have at least 1 of the following**:
  - Capital Letin letters (A-Z)
  - Small Latin letters (a-z)
  - Digits (0-9)
  - Valid special characters
- Valid special characters are: `~ ! @ # $ % ^ & * ( ) - _ + = { } [ ] \ | ; : " ' < > , . / ?`
  - Hint: If you didn't notice, they are all valid ASCII characters.
- If any of the above **4** conditions are violated, return `0`, else return `1`.

#### Examples

Invalid passwords (return 0)

- `Abc123!`
  - Too short (7 characters long)
- `AbCdEfGhIjKlMn0P!!!!!!!!!`
  - Too long (25 characters long)
- `helloworld123`
  - No uppercase letter or special character
- `He!l0 world`
  - Whitespace is not a valid special character

Valid passwords (return 1)

- S0ftwareTutorial*
- H3!!0world

#### Assumptions

- All test cases will only use valid ASCII characters.
- The test cases will be at most 512 characters long.

### ii) Email checker

This one will have slightly more complicated rules, but does the same thing basically.

```int email_check(char* email)```

Complete the function above in `Task1.c`.

The function will process a C string `email` and return 0 or 1 based on whether the email is valid or not.
The function will also print out the email address the mail is sent to.

Disclaimer: some of the parts of the rules is kinda obsolete in the official modern standards, but it'll be fun to test your skills :\)

#### Rules (The rules are loosely based on the actual RFC 5322 standards.)

#### Validation

The email address must follow the format of `<local-part>@<domain>`, where the `<>` can be replaced by a string of characters.

##### \<local-part\>

The `local-part` of an email address basically indicates who the email will be sent to (basically the receiver of the email).
The email sent to the email address will be sent to the mailbox with the name indicated in `local-part`, after removing quotes, tags, and comments (those will be mentioned later).

The `local-part` must follow the rules below:

- The length of the local-part is in the range 1-64 (inclusive) (including quotations and comments)
- The length of the **VALID** mailbox indicated by the local-part must not be empty. (See output for definition of mailbox)

**Unquoted**:

- The local-part is defined as 'unquoted' if it does not start and end with the `"` character.
- The following rules apply when the local-part is unquoted:
  - It may include any capital/small Latin letter, and any digit.
  - It may include the characters ```! # $ % & ' * + - / = ? ^ _ ` { | } ~```
  - The `.` character can be included, but it cannot be leading/trailing, nor can they appear consecutively.
    - (`.username@email.com`,`username.@email.com`,`user..name@email.com` are all **not** allowed)

**Quoted**:

- The local-part is definted as `quoted` if the entire string **BOTH** starts and ends with the `"` character. (e.g. `"username"@email.com`)
- The following rules apply to the quoted string (the part quoted by the `"` characters excluding the outer `"` characters) when the local-part is **quoted**:
  - It may include any characters allowed in the unquoted restrictions.
  - The characters `" ( ) , : ; < > @ [ \ ]` are also allowed.
    - For the characters `"` and `\` to be used, they must be immediately preceeded by a `\` operator. (`"user"name\lol"@email.com` is invalid, but `"user\"name\\me"@email.com` is valid.)
    - Whitespaces are allowed if they are not immedietely preceded by the '\' operator (`\\ `@email.com is valid, but `\\\ `@email.com is valid).
    - The `\` operator is not a part the mailbox, and will be removed during the output phase.

Tags:

- Tags only exist in **unquoted local-parts** and defined by a string starting with a `+` character. (e.g. `username+tag@email.com`, where `username` is the mailbox and `tag` is the tag)
- There can only be 1 tag in an email. (e.g. `username+tag+tag@gmail.com` has a tag of `tag+tag` instead of having 2 tags.)
- The contents of the tag must also follow the restrictions of the unquoted part.
- Tags are not a part of the mailbox, and will be removed during the output phase. The contents of the tag must also follow the restrictions of the unquoted part.

##### \<domain\>

The `domain` of an email address is basically the address of the receiver.

The `domain` must follow the rules below:

- The domain must be at most 253 characters. (<=253)
- The domain must only include capital/small Latin letters, digits, hyphens `-`, and dots `.`.
- The domain is separated by dots `.` into subdomains.
- Subdomain rules:
  - Subdomains must not be empty.
  - They must not start or end in hyphens `-`.
  - Subdomains must be at most 63 characters. (<=63)
  - The last subdomain (the most right most one, also known as the top-level domain) must be at least 2 characters long, and can only include letters.

#### Output

The output of the f

The function should output the following: (if the input of the function is `username@email.com`)

```console
The email username@email.com is valid.
Mailbox: username
Domain: email.com
```

Unquoted local-parts and domains are **case insensitive**. So the output will reflect that: (if the input of the function is `HelloWorld@Email.Com`)

```console
The email HelloWorld@Email.Com is valid.
Mailbox: helloworld
Domain: email.com
```

Quoted local-parts will take the literal string inside quotations as the mailbox, excluding all backslash operators: (if the input of the function is `"h$>>o\"WOrld\\"@gmail.com`)

```console
The email "h$>>o\"WOrld\\"@gmail.com is valid.
Mailbox: h$>>o"WOrld\
Domain: gmail.com
```

For invalid emails: (if the input of the function is `"l"l"@email.com`, which is invalid due to a `"` without a '\' operator)

```console
The email "l"l"@email.com is invalid.
```

If the input is `hello@world.123` (invalid due to the top-level domain rule)

```console
The email hello@world.123 is invalid.
```

##### Comments

Comments can be included in the email address, in both the local-part and the domain part, but not across.  
They are defined by starting with a `(` character, and ending with a `)` character.  
Comments cannot exist within quoted local-parts. (For the email `"user(n)ame"@email.com`, the `(n)` is not treated as a comment, but as a literal string.)

The content inside comments will be ignored during both the validation step and the output step.

Examples:

Input: `fu(happy)n+things@yahoo.com`

```console
The email fu(happy)n+things@yahoo.com is valid.
Mailbox: fun
Domain: yahoo.com
```

Input: `"foo+bar(n)"@abc.com.uk`

```console
The email "foo+bar(n)"@abc.com.uk is valid.
Mailbox: foo+bar(n)
Domain: abc.com.uk
```
