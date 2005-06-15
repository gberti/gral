%{
#include <iostream>
#include "Gral/Generation/Triangle/TriangleOptionsParser_classdecl.h"

#define YY_USER_INIT at_char=-1
#define YY_USER_ACTION at_char+=YYLeng();

%}
%option prefix="Triangle"
%option noyywrap
%option yylineno
%option yyclass="TriangleOptionsParser"

pm [+-]
digit	[0-9]
exp	[eE][+-]?{digit}+


plg                p
refine             r
quality            q
maxarea            a
userconstraint     u
inellipse          T
attributes         A
convexhull         c
weighted           w
regular            W
jettison           j
edgelist           e
voronoi            v
neighbours         n
geomview           g
noboundinfo        B
nopolyfile         P
nonodefile         N
noelefile          E
noiterinfo         I
ignoreholes        O
noexactarithm      X
zerobasednum       z
secondorder        o2 
noboundsplit       Y
steiner            S
equatorialcircles  L
incremental        i
sweepline          F
verticalcuts       l
forcebysplit       s
checkconsistency   C
quiet              Q
verbose            V

%%

{plg}                     {plg(); }
{refine}                  {refine(); }
{quality}                 {quality(); }
{quality}{pm}?{digit}+		       	 {quality(YYText());}
{quality}{pm}?{digit}+{exp}		       	 {quality(YYText());}
{quality}{pm}?{digit}+\.{digit}*{exp}?  {quality(YYText());}
{quality}{pm}?{digit}*\.{digit}+{exp}?  {quality(YYText());}

{maxarea}                 {maxarea(); }
{maxarea}{pm}?{digit}+		       	 {maxarea(YYText());}
{maxarea}{pm}?{digit}+{exp}		       	 {maxarea(YYText());}
{maxarea}{pm}?{digit}+\.{digit}*{exp}?  {maxarea(YYText());}
{maxarea}{pm}?{digit}*\.{digit}+{exp}?  {maxarea(YYText());}

{userconstraint}          {userconstraint(); }
{inellipse}               {inellipse();}
{attributes}              {attributes(); }
{convexhull}              {convexhull(); }
{weighted}                {weighted(); }
{regular}                 {regular(); }
{jettison}                {jettison(); }
{edgelist}                {edgelist(); }
{voronoi}                 {voronoi(); }
{neighbours}              {neighbours(); }
{geomview}                {geomview(); }
{noboundinfo}             {noboundinfo(); }
{nopolyfile}              {nopolyfile(); }
{nonodefile}              {nonodefile(); }
{noelefile}               {noelefile(); }
{noiterinfo}              {noiterinfo(); }
{ignoreholes}             {ignoreholes(); }
{noexactarithm}           {noexactarithm(); }
{zerobasednum}            {zerobasednum(); }
{secondorder}             {secondorder(); }
{noboundsplit}            {noboundsplit(); }
{steiner}?{digit}+		    {steiner(YYText());}
{steiner}         		    {steiner();}
{equatorialcircles}       {equatorialcircles(); }
{incremental}             {incremental(); }
{sweepline}               {sweepline(); }
{verticalcuts}            {verticalcuts(); }
{forcebysplit}            {forcebysplit(); }
{checkconsistency}        {checkconsistency(); }
{quiet}                   {quiet();}
{verbose}                 {verbose();}
- { parseminus();}
[ \t\n\r]+				{ /* ignore white characters */}
.  {invalid_input(YYText());}
<<EOF>> {return 0;}
%%

