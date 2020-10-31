# Classification of combinations

## Theoretical introduction

 On modern electronic financial markets, many types of financial instruments are traded - stocks (share), bonds (bond), futures (future contracts) and forward contracts (forward contracts), various types of options, etc. Combinations are a separate type of financial instruments.
 _Combination_ is a composite financial instrument consisting of many instruments and their associated weights:
 
![C=(I_1, w_1), (I_2, w_2), \.\.\., (I_N, w_N)](https://render.githubusercontent.com/render/math?math=C%3D(I_1%2C%20w_1)%2C%20(I_2%2C%20w_2)%2C%20%5C.%5C.%5C.%2C%20(I_N%2C%20w_N))

where ![I_1, I_2, \.\.\. , I_N](https://render.githubusercontent.com/render/math?math=I_1%2C%20I_2%2C%20%5C.%5C.%5C.%20%2C%20I_N) - arbitrary financial instruments (_components_), and ![w_1, w_2, \. \. \. , w_N](https://render.githubusercontent.com/render/math?math=w_1%2C%20w_2%2C%20%5C.%5C.%5C.%20%2C%20w_N) - the corresponding weights ( ![w_1, w_2, \. \. \., w_N \ in R](https://render.githubusercontent.com/render/math?math=w_1%2C%20w_2%2C%20%5C.%5C.%5C.%20%2C%20w_N%20%5Cin%20R)). Buying (selling) a combination entails buying (selling) a positive weight component and selling (buying) a negative weight component. A simple example of a combination would be a stock index.
                                                                                                                                    
                                                                                                                                     Further, it will be useful to know what an option is. _Option_ is a financial instrument that gives its owner the right to buy (call option, call option) or sell (put option, put option) an underlying asset (for example, a share) at a certain price (strike price) at a certain moment in the future (date execution, expiration date).
                                                                                                                                    
                                                                                                                                     Combinations, like other financial instruments, can be of various types. For example, a combination of one call and one put options with the same prices and expiration dates is called a straddle. And if the strike price of the put option is less than the strike price of the call option (the expiration dates are the same), then the combination is called strangle.
## Formulation of the problem

 The task is to determine the type of combination by its structure and properties of the components.
## Resource format description

Many of all types are described as an XML resource. Each combination is described by a tag &lt;combination&gt;:

```xml
<combination name="Straddle" shortname="Str" identifier="da5a2620-575b-11df-9029-c3f2a99ff460">
	<!-- ... -->
</combination>
```

Required tag attributes &lt;combination&gt; are the fully qualified name of the combination type (name), short name (shortname) и UUID identifier (identifier).
Inside the tag &lt;combination&gt; describes the components of the combination:

```xml
<combination name="Straddle" shortname="Str" identifier="da5a2620-575b-11df-9029-c3f2a99ff460">
	<legs cardinality="fixed">
		<!-- ... -->
	</legs>
</combination>
```

In this &lt;legs&gt; there is a required attribute - the number of components in combinations of this type (cardinality). There are several possible values for the attribute:
1. **fixed**: the number of components is fixed and is equal to the number described in the resource.
3. **multiple**: the number of components is a multiple of the number described in the resource.
4. **more**: the number of components must be greater than or equal to that specified in the attribute **mincount**

Inside the tag &lt;legs&gt; the components of the combinations of the corresponding type are described:

```xml
<combination name="Straddle" shortname="Str" identifier="da5a2620-575b-11df-9029-c3f2a99ff460">
	<legs cardinality="fixed">
		<leg type="C" ratio="1" strike="X" expiration="X">
		<leg type="P" ratio="1" strike="X" expiration="X">
	</legs>
</combination>
```

Each tag &lt;leg&gt; has required attributes:
1. **type**: the type of instrument of the corresponding component is described by a letter from the set (**F** - futures, **O** - option, **C** - call option, **P** - put option, **U** - instrument other than option). put (**P**) и call (**C**) options are special cases of options (**O**).

2. **ratio:** the weight of the corresponding component is described by an arbitrary real number. It is allowed to indicate both the exact weight and just its sign - &quot;+&quot; или &quot;-&quot;, meaning that the weight is positive or negative.

Each leg can have additional attributes describing the properties of the component:
1. **strike**: the strike price is indicated by a capital letter of the Latin alphabet. Those components for which this attribute is set and the letter designations are equal must have the same strike price. The attribute can only be specified for options.

2. **strike\_offset**: describes how much the component strike price should be higher / lower than that of the previous option, for which this attribute is not described. It is a string of characters &quot;+&quot; or a string of characters &quot;-&quot;. For a visual explanation, here's an example:
```xml
	<!-- ... -->
                <leg type="P" ratio="1" />
                <leg type="P" ratio="1" strike_offset="+"/>
                <leg type="P" ratio="1" strike_offset="+"/>
                <leg type="P" ratio="1" strike_offset="++"/>
                <leg type="С" ratio="1" />
                <leg type="С" ratio="1" strike_offset="-"/>
	<!-- ... -->
```
In this example, the strike price of the first component can be arbitrary, the strike prices of the second and third components must be the same (equal lengths mean the same offset), but higher than the first. The strike price of the fourth component must be greater than the strike prices of the second and third. The execution price of the fifth component can be arbitrary. The strike price of the sixth component must be less than the strike price of the fifth. The attribute can only be specified for options.

3. **expiration**: is similar to the strike attribute, describing the expiration date of the instrument in the same format.

4. **expiration\_offset**: almost identical to the strike\_ffset attribute. In addition to lines of symbols &quot;+&quot; and &quot;-&quot; may contain a specific offset in the form &quot;[N]{d,m,q,y}&quot;, where N is a natural number (can be omitted, by default it is equal to one), &quot;d&quot; - day, &quot;m&quot; - month, &quot;q&quot; - quarter, &quot;y&quot; - year. Note, что expiraion\_offset=&quot;q&quot; Unlike expiraion\_offset=&quot;3m&quot; means the expiration date is not exactly three months (day to day), but the expiration in the next quarter, i.e. the day may not exactly coincide, the main thing is that the difference in months is equal to three.

XML-a resource describing the types of combinations is located in the repository.

## Input output format

 The first line contains the number N - the number of components (2 ≤ N ≤ 100000). Then in N lines there is a description of the components:

type weight expiration strike, где type - символ &quot;C&quot;, &quot;P&quot;, &quot;O&quot;, &quot;F&quot;, &quot;U&quot;, weight - of component, expiration - date of expiration if format yyyy-mm-dd, strike - execution price. The strike price is set only for options.

 If the combination was classified on the first line, print the full name of the combination type. The following lines should contain the permutation of numbers from 1 to N - the order of the components in which they should be considered (according to the order described in the resource).

 If the combination was not classified, you should output one line - &quot;Unclassified&quot;.

If it is not possible to unambiguously determine the type of combination, then the type that is described in the resource earlier has priority.

## Example

| **Input** | **Output** |
| --- | --- |
| 2<br>P 1.0 100 2013-10-19<br>C 1.0 100 2013-10-19 | Straddle<br>2<br>1 |
| 3<br>F 1.0 2013-10-19<br>F -2.0 2013-11-16<br>F 1.0 2013-12-21 | Future butterfly<br>1<br>2<br>3|
| 2<br>P 1.0 100 2013-10-18<br>C 1.0 100 2013-10-19 | Unclassified |

