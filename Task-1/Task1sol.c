//Reference solution for Task 1.
//Compile with: gcc main.c Task1sol.c -o main
#include "Task1.h"

#include <stdio.h>
#include <string.h>

//==================== shared helpers ====================
static int is_digit(char c){ return c>='0' && c<='9'; }
static int is_upper(char c){ return c>='A' && c<='Z'; }
static int is_lower(char c){ return c>='a' && c<='z'; }
static int is_alpha(char c){ return is_upper(c) || is_lower(c); }
static int is_alnum(char c){ return is_alpha(c) || is_digit(c); }

//Characters allowed in an unquoted local-part (letters, digits, these specials, '.').
static int is_unquoted_special(char c){
    static const char* sp = "!#$%&'*+-/=?^_`{|}~";
    for(int i=0; sp[i]!='\0'; i++) if(c==sp[i]) return 1;
    return 0;
}
static int is_valid_unquoted_char(char c){
    return is_alnum(c) || is_unquoted_special(c) || c=='.';
}

//Characters allowed as plain text inside a quoted local-part.
//('"' and '\\' are NOT here - they may only appear through escapes.)
static int is_valid_quoted_char(char c){
    if(c==' ') return 1;
    if(is_valid_unquoted_char(c)) return 1;
    static const char* extra = "(),:;<>@[]";
    for(int i=0; extra[i]!='\0'; i++) if(c==extra[i]) return 1;
    return 0;
}

//Removes ( ... ) comments; returns 1 on success, 0 on an unterminated comment.
static int strip_comments(char* src, char* dst){
    int i=0, j=0;
    while(src[i]!='\0'){
        if(src[i]=='('){
            i++;
            while(src[i]!='\0' && src[i]!=')') i++;
            if(src[i]=='\0') return 0;
            i++;
        }
        else{
            dst[j++]=src[i++];
        }
    }
    dst[j]='\0';
    return 1;
}

//Finds the main '@' separating local-part and domain, skipping comments and the
//leading quoted section (so an '@' inside them does not count).
static int find_at(char* s){
    int i=0;
    if(s[0]=='"'){
        i=1;
        while(s[i]!='\0'){
            if(s[i]=='\\'){ i+=2; continue; }
            if(s[i]=='"'){ i++; break; }
            i++;
        }
    }
    while(s[i]!='\0'){
        if(s[i]=='('){
            i++;
            while(s[i]!='\0' && s[i]!=')') i++;
            if(s[i]==')') i++;
            continue;
        }
        if(s[i]=='@') return i;
        i++;
    }
    return -1;
}

static void lower_str(char* s){
    for(int i=0; s[i]!='\0'; i++){
        if(is_upper(s[i])) s[i]+=32;
    }
}

//Validates a (comment-stripped) domain string.
static int valid_domain(char* d){
    if(d[0]=='\0') return 0;
    if((int)strlen(d) > 253) return 0;
    for(int i=0; d[i]!='\0'; i++){
        char c=d[i];
        if(!(is_alnum(c) || c=='-' || c=='.')) return 0;
    }
    int n=(int)strlen(d);
    if(d[0]=='.' || d[n-1]=='.') return 0;
    int start=0;
    for(int i=0;i<=n;i++){
        if(i==n || d[i]=='.'){
            int len=i-start;
            int is_last=(i==n);
            if(len==0 || len>63) return 0;          //empty or too long subdomain
            if(d[start]=='-' || d[i-1]=='-') return 0; //hyphen at a boundary
            if(is_last){
                if(len<2) return 0;                 //TLD too short
                for(int j=start;j<i;j++){
                    if(!is_alpha(d[j])) return 0;   //TLD must be all letters
                }
            }
            start=i+1;
        }
    }
    return 1;
}

//==================== Part A (i): password ====================
int password_check(char* password){
    int len=0;
    while(password[len]!='\0') len++;
    if(len<8 || len>24) return 0;
    static const char* specials = "~!@#$%^&*()-_+={}[]\\|;:\"'<>,./?";
    int has_upper=0, has_lower=0, has_digit=0, has_special=0;
    for(int i=0;i<len;i++){
        char c=password[i];
        if(is_upper(c)) has_upper=1;
        else if(is_lower(c)) has_lower=1;
        else if(is_digit(c)) has_digit=1;
        else if(strchr(specials, c)!=NULL) has_special=1;
        else return 0; //not an allowed character
    }
    return has_upper && has_lower && has_digit && has_special;
}

//==================== Part A (ii): email ====================
void email_check(char* email){
    char mailbox[128];
    char domain[128];
    char local[128];
    char dom[128];
    char stripped[128];

    int at=find_at(email);
    if(at<=0) goto invalid;

    int i;
    int ll=at;
    for(i=0;i<ll;i++) local[i]=email[i];
    local[ll]='\0';
    int dl=0;
    for(i=at+1; email[i]!='\0'; i++) dom[dl++]=email[i];
    dom[dl]='\0';

    if(ll<1 || ll>64) goto invalid;  //raw local-part length
    if(dl>253) goto invalid;         //raw domain length

    //---- domain ----
    if(!strip_comments(dom, domain)) goto invalid;
    if(!valid_domain(domain)) goto invalid;
    lower_str(domain);

    //---- local-part ----
    if(local[0]=='"'){
        //Quoted: must also end with a quote.
        if(local[ll-1]!='"') goto invalid;
        int mi=0;
        for(i=1;i<ll-1;i++){
            char c=local[i];
            if(c=='\\'){
                if(i+1>=ll-1) goto invalid;         //trailing backslash
                char n=local[i+1];
                if(n=='"' || n=='\\'){
                    mailbox[mi++]=n;                //escaped literal
                    i++;
                }
                else goto invalid;
            }
            else if(c=='"'){
                goto invalid;                        //unescaped quote
            }
            else{
                if(!is_valid_quoted_char(c)) goto invalid;
                mailbox[mi++]=c;
            }
        }
        mailbox[mi]='\0';
        if(mailbox[0]=='\0') goto invalid;           //empty mailbox
    }
    else{
        //Unquoted: strip comments first.
        if(!strip_comments(local, stripped)) goto invalid;
        for(i=0; stripped[i]!='\0'; i++){
            if(!is_valid_unquoted_char(stripped[i])) goto invalid;
        }
        int n=(int)strlen(stripped);
        if(n==0) goto invalid;
        if(stripped[0]=='.' || stripped[n-1]=='.') goto invalid;
        for(i=0;i<n-1;i++){
            if(stripped[i]=='.' && stripped[i+1]=='.') goto invalid;
        }
        //Mailbox = everything before the first '+', lowercased.
        int mi=0;
        for(i=0; stripped[i]!='\0' && stripped[i]!='+'; i++){
            mailbox[mi++]=stripped[i];
        }
        mailbox[mi]='\0';
        if(mailbox[0]=='\0') goto invalid;
        lower_str(mailbox);
    }

    printf("The email %s is valid.\n", email);
    printf("Mailbox: %s\n", mailbox);
    printf("Domain: %s\n", domain);
    return;

invalid:
    printf("The email %s is invalid.\n", email);
}

//==================== Part B: regex ====================
static int is_quant(char c){
    return c=='*'||c=='+'||c=='?'||c=='{';
}

static int match_range(const char* re, int ri, int re_end, const char* str, int si);

//Matches a single "unit" (literal, '.', class, or group) starting at re[unit_start]
//against str[si]. Returns the number of chars consumed, or -1. Sets *unit_end to
//the regex index just after the unit.
static int try_unit(const char* re, int unit_start, const char* str, int si, int* unit_end){
    char c=re[unit_start];
    if(c=='('){
        int j=unit_start+1;
        while(re[j]!='\0' && re[j]!=')') j++;
        if(re[j]!=')') return -1;
        *unit_end=j+1;
        return match_range(re, unit_start+1, j, str, si);
    }
    if(c=='['){
        int close=unit_start+1;
        while(re[close]!='\0' && re[close]!=']') close++;
        if(re[close]!=']') return -1;
        *unit_end=close+1;
        int negated=0;
        int j=unit_start+1;
        if(re[j]=='^'){ negated=1; j++; }
        char test=str[si];
        if(test=='\0') return -1;
        int matched=0;
        while(j<close){
            if(j+2<close && re[j+1]=='-'){
                char lo=re[j], hi=re[j+2];
                if(test>=lo && test<=hi) matched=1;
                j+=3;
            }
            else{
                if(test==re[j]) matched=1;
                j++;
            }
        }
        int ok = negated ? !matched : matched;
        return ok ? 1 : -1;
    }
    *unit_end=unit_start+1;
    if(c=='.'){
        if(str[si]=='\0') return -1;
        return 1;
    }
    if(str[si]!='\0' && c==str[si]) return 1;
    return -1;
}

//Parses a quantifier at re[q] into [min, max] (max=-1 = unlimited) and sets
//*after to the index just past the quantifier.
static void parse_quant(const char* re, int q, int* min, int* max, int* after){
    if(re[q]=='{'){
        int j=q+1;
        int m1=0;
        while(re[j]>='0' && re[j]<='9'){ m1=m1*10+(re[j]-'0'); j++; }
        if(re[j]=='}'){ *min=m1; *max=m1; *after=j+1; return; }
        if(re[j]==','){
            j++;
            if(re[j]=='}'){ *min=m1; *max=-1; *after=j+1; return; }
            int m2=0;
            while(re[j]>='0' && re[j]<='9'){ m2=m2*10+(re[j]-'0'); j++; }
            *min=m1; *max=m2; *after=j+1; return;
        }
    }
    else if(re[q]=='*'){ *min=0; *max=-1; *after=q+1; return; }
    else if(re[q]=='+'){ *min=1; *max=-1; *after=q+1; return; }
    else if(re[q]=='?'){ *min=0; *max=1; *after=q+1; return; }
    *min=0; *max=0; *after=q+1; //fallback (valid regexes never reach here)
}

//Backtracking for a quantified unit. 'pos' is the string position after the
//minimum repetitions have already been consumed. Greedy: tries more repetitions
//first, then backs off.
static int match_quant_rec(const char* re, int unit_start, int after, int re_end,
                           int min, int max, const char* str, int si, int pos){
    int taken=pos-si;
    if(max<0 || taken<max){
        int ue;
        int v=try_unit(re, unit_start, str, pos, &ue);
        if(v>0){
            int r=match_quant_rec(re, unit_start, after, re_end, min, max, str, si, pos+v);
            if(r>=0) return r;
        }
    }
    int rest=match_range(re, after, re_end, str, pos);
    if(rest>=0) return (pos-si)+rest;
    return -1;
}

//Matches re[ri..re_end) against str[si..]; returns chars consumed or -1.
static int match_range(const char* re, int ri, int re_end, const char* str, int si){
    if(ri>=re_end) return 0;
    int unit_end;
    int u=try_unit(re, ri, str, si, &unit_end);
    if(u<0) return -1;
    if(unit_end<re_end && is_quant(re[unit_end])){
        int min, max, after;
        parse_quant(re, unit_end, &min, &max, &after);
        int pos=si;
        for(int k=0;k<min;k++){
            int ue;
            int v=try_unit(re, ri, str, pos, &ue);
            if(v<0) return -1;
            pos+=v;
        }
        return match_quant_rec(re, ri, after, re_end, min, max, str, si, pos);
    }
    else{
        int rest=match_range(re, unit_end, re_end, str, si+u);
        if(rest<0) return -1;
        return u+rest;
    }
}

void regex(char** list, char* re){
    printf("Regex = %s\n", re);
    printf("Results:\n");
    int re_len=(int)strlen(re);
    for(int i=0; list[i]!=NULL; i++){
        const char* s=list[i];
        int consumed=match_range(re, 0, re_len, s, 0);
        if(consumed==(int)strlen(s)){
            printf("%s\n", s);
        }
    }
}
