<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="9.4.2">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="15" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="6" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="88" name="SimResults" color="9" fill="1" visible="yes" active="yes"/>
<layer number="89" name="SimProbes" color="9" fill="1" visible="yes" active="yes"/>
<layer number="90" name="Modules" color="5" fill="1" visible="yes" active="yes"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
<layer number="99" name="SpiceOrder" color="5" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="relay" urn="urn:adsk.eagle:library:339">
<description>&lt;b&gt;Relays&lt;/b&gt;&lt;p&gt;
&lt;ul&gt;
&lt;li&gt;Eichhoff
&lt;li&gt;Finder
&lt;li&gt;Fujitsu
&lt;li&gt;HAMLIN
&lt;li&gt;OMRON
&lt;li&gt;Matsushita
&lt;li&gt;NAiS
&lt;li&gt;Siemens
&lt;li&gt;Schrack
&lt;/ul&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="G2R" urn="urn:adsk.eagle:footprint:23988/1" library_version="4">
<description>&lt;b&gt;RELAY&lt;/b&gt;&lt;p&gt;
1 x switch, Omron</description>
<wire x1="-4.699" y1="-6.604" x2="24.384" y2="-6.604" width="0.1524" layer="21"/>
<wire x1="24.384" y1="6.477" x2="24.384" y2="-6.604" width="0.1524" layer="21"/>
<wire x1="24.384" y1="6.477" x2="-4.699" y2="6.477" width="0.1524" layer="21"/>
<wire x1="-4.699" y1="-6.604" x2="-4.699" y2="6.477" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="2.0574" x2="-2.54" y2="1.1938" width="0.1524" layer="21"/>
<wire x1="-0.6604" y1="1.1938" x2="-0.6604" y2="-1.3208" width="0.254" layer="21"/>
<wire x1="-4.4196" y1="-1.3208" x2="-4.4196" y2="1.1938" width="0.254" layer="21"/>
<wire x1="-4.4196" y1="1.1938" x2="-2.54" y2="1.1938" width="0.254" layer="21"/>
<wire x1="-2.54" y1="1.1938" x2="-1.2954" y2="1.1938" width="0.254" layer="21"/>
<wire x1="-3.7846" y1="-1.3208" x2="-4.4196" y2="-1.3208" width="0.254" layer="21"/>
<wire x1="-1.2954" y1="1.1938" x2="-3.7846" y2="-1.3208" width="0.1524" layer="21"/>
<wire x1="-1.2954" y1="1.1938" x2="-0.6604" y2="1.1938" width="0.254" layer="21"/>
<wire x1="-0.6604" y1="-1.3208" x2="-2.54" y2="-1.3208" width="0.254" layer="21"/>
<wire x1="-2.54" y1="-1.3208" x2="-2.54" y2="-1.9304" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="-1.3208" x2="-3.7846" y2="-1.3208" width="0.254" layer="21"/>
<wire x1="16.256" y1="-0.508" x2="16.5862" y2="-0.1778" width="0.254" layer="21"/>
<wire x1="16.5862" y1="-0.1778" x2="13.97" y2="-0.1778" width="0.1524" layer="21"/>
<wire x1="16.5862" y1="-0.1778" x2="17.526" y2="0.762" width="0.254" layer="21"/>
<wire x1="13.97" y1="-0.1778" x2="13.97" y2="-1.8288" width="0.1524" layer="21"/>
<wire x1="18.4912" y1="-0.1778" x2="20.955" y2="-0.1778" width="0.1524" layer="21"/>
<wire x1="20.955" y1="-0.1778" x2="20.955" y2="-1.8288" width="0.1524" layer="21"/>
<wire x1="17.526" y1="1.905" x2="17.526" y2="0.762" width="0.1524" layer="21"/>
<pad name="2" x="-2.54" y="3.81" drill="1.3208" shape="long"/>
<pad name="1" x="-2.54" y="-3.683" drill="1.3208" shape="long"/>
<pad name="P" x="17.4498" y="3.81" drill="1.3208" shape="long"/>
<pad name="O" x="13.97" y="-3.683" drill="1.3208" shape="long"/>
<pad name="S" x="20.955" y="-3.683" drill="1.3208" shape="long"/>
<text x="26.6446" y="-6.2992" size="1.778" layer="25" ratio="10" rot="R90">&gt;NAME</text>
<text x="3.81" y="-5.08" size="1.778" layer="27" ratio="10" rot="R90">&gt;VALUE</text>
</package>
</packages>
<packages3d>
<package3d name="G2R" urn="urn:adsk.eagle:package:24323/1" type="box" library_version="4">
<description>RELAY
1 x switch, Omron</description>
<packageinstances>
<packageinstance name="G2R"/>
</packageinstances>
</package3d>
</packages3d>
<symbols>
<symbol name="K" urn="urn:adsk.eagle:symbol:23941/1" library_version="4">
<wire x1="-3.81" y1="-1.905" x2="-1.905" y2="-1.905" width="0.254" layer="94"/>
<wire x1="3.81" y1="-1.905" x2="3.81" y2="1.905" width="0.254" layer="94"/>
<wire x1="3.81" y1="1.905" x2="1.905" y2="1.905" width="0.254" layer="94"/>
<wire x1="-3.81" y1="1.905" x2="-3.81" y2="-1.905" width="0.254" layer="94"/>
<wire x1="0" y1="-2.54" x2="0" y2="-1.905" width="0.1524" layer="94"/>
<wire x1="0" y1="-1.905" x2="3.81" y2="-1.905" width="0.254" layer="94"/>
<wire x1="0" y1="2.54" x2="0" y2="1.905" width="0.1524" layer="94"/>
<wire x1="0" y1="1.905" x2="-3.81" y2="1.905" width="0.254" layer="94"/>
<wire x1="-1.905" y1="-1.905" x2="1.905" y2="1.905" width="0.1524" layer="94"/>
<wire x1="-1.905" y1="-1.905" x2="0" y2="-1.905" width="0.254" layer="94"/>
<wire x1="1.905" y1="1.905" x2="0" y2="1.905" width="0.254" layer="94"/>
<text x="1.27" y="2.921" size="1.778" layer="96">&gt;VALUE</text>
<text x="1.27" y="5.08" size="1.778" layer="95">&gt;PART</text>
<pin name="2" x="0" y="-5.08" visible="pad" length="short" direction="pas" rot="R90"/>
<pin name="1" x="0" y="5.08" visible="pad" length="short" direction="pas" rot="R270"/>
</symbol>
<symbol name="U" urn="urn:adsk.eagle:symbol:23944/1" library_version="4">
<wire x1="3.175" y1="5.08" x2="1.905" y2="5.08" width="0.254" layer="94"/>
<wire x1="-3.175" y1="5.08" x2="-1.905" y2="5.08" width="0.254" layer="94"/>
<wire x1="0" y1="1.27" x2="2.54" y2="5.715" width="0.254" layer="94"/>
<wire x1="0" y1="1.27" x2="0" y2="0" width="0.254" layer="94"/>
<circle x="0" y="1.27" radius="0.127" width="0.4064" layer="94"/>
<text x="2.54" y="0" size="1.778" layer="95">&gt;PART</text>
<pin name="O" x="5.08" y="5.08" visible="pad" length="short" direction="pas" rot="R180"/>
<pin name="S" x="-5.08" y="5.08" visible="pad" length="short" direction="pas"/>
<pin name="P" x="0" y="-2.54" visible="pad" length="short" direction="pas" rot="R90"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="G2R" urn="urn:adsk.eagle:component:24612/1" prefix="K" library_version="4">
<description>&lt;b&gt;RELAY&lt;/b&gt;&lt;p&gt;
1 x switch, Omron</description>
<gates>
<gate name="1" symbol="K" x="0" y="0" addlevel="must"/>
<gate name="2" symbol="U" x="17.78" y="0" addlevel="always"/>
</gates>
<devices>
<device name="" package="G2R">
<connects>
<connect gate="1" pin="1" pad="1"/>
<connect gate="1" pin="2" pad="2"/>
<connect gate="2" pin="O" pad="O"/>
<connect gate="2" pin="P" pad="P"/>
<connect gate="2" pin="S" pad="S"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:24323/1"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="MF" value="" constant="no"/>
<attribute name="MPN" value="" constant="no"/>
<attribute name="OC_FARNELL" value="1569185" constant="no"/>
<attribute name="OC_NEWARK" value="unknown" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="supply2" urn="urn:adsk.eagle:library:372">
<description>&lt;b&gt;Supply Symbols&lt;/b&gt;&lt;p&gt;
GND, VCC, 0V, +5V, -5V, etc.&lt;p&gt;
Please keep in mind, that these devices are necessary for the
automatic wiring of the supply signals.&lt;p&gt;
The pin name defined in the symbol is identical to the net which is to be wired automatically.&lt;p&gt;
In this library the device names are the same as the pin names of the symbols, therefore the correct signal names appear next to the supply symbols in the schematic.&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
</packages>
<symbols>
<symbol name="+12V" urn="urn:adsk.eagle:symbol:26985/1" library_version="2">
<wire x1="0" y1="1.905" x2="0" y2="0.635" width="0.1524" layer="94"/>
<wire x1="-0.635" y1="1.27" x2="0.635" y2="1.27" width="0.1524" layer="94"/>
<circle x="0" y="1.27" radius="1.27" width="0.254" layer="94"/>
<text x="-2.54" y="3.175" size="1.778" layer="96">&gt;VALUE</text>
<pin name="+12V" x="0" y="-2.54" visible="off" length="short" direction="sup" rot="R90"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="+12V" urn="urn:adsk.eagle:component:27033/1" prefix="SUPPLY" library_version="2">
<description>&lt;b&gt;SUPPLY SYMBOL&lt;/b&gt;</description>
<gates>
<gate name="+12V" symbol="+12V" x="0" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="supply1" urn="urn:adsk.eagle:library:371">
<description>&lt;b&gt;Supply Symbols&lt;/b&gt;&lt;p&gt;
 GND, VCC, 0V, +5V, -5V, etc.&lt;p&gt;
 Please keep in mind, that these devices are necessary for the
 automatic wiring of the supply signals.&lt;p&gt;
 The pin name defined in the symbol is identical to the net which is to be wired automatically.&lt;p&gt;
 In this library the device names are the same as the pin names of the symbols, therefore the correct signal names appear next to the supply symbols in the schematic.&lt;p&gt;
 &lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
</packages>
<symbols>
<symbol name="GND" urn="urn:adsk.eagle:symbol:26925/1" library_version="1">
<wire x1="-1.905" y1="0" x2="1.905" y2="0" width="0.254" layer="94"/>
<text x="-2.54" y="-2.54" size="1.778" layer="96">&gt;VALUE</text>
<pin name="GND" x="0" y="2.54" visible="off" length="short" direction="sup" rot="R270"/>
</symbol>
<symbol name="+12V" urn="urn:adsk.eagle:symbol:26931/1" library_version="1">
<wire x1="1.27" y1="-1.905" x2="0" y2="0" width="0.254" layer="94"/>
<wire x1="0" y1="0" x2="-1.27" y2="-1.905" width="0.254" layer="94"/>
<wire x1="1.27" y1="-0.635" x2="0" y2="1.27" width="0.254" layer="94"/>
<wire x1="0" y1="1.27" x2="-1.27" y2="-0.635" width="0.254" layer="94"/>
<text x="-2.54" y="-5.08" size="1.778" layer="96" rot="R90">&gt;VALUE</text>
<pin name="+12V" x="0" y="-2.54" visible="off" length="short" direction="sup" rot="R90"/>
</symbol>
<symbol name="VCC" urn="urn:adsk.eagle:symbol:26928/1" library_version="1">
<wire x1="1.27" y1="-1.905" x2="0" y2="0" width="0.254" layer="94"/>
<wire x1="0" y1="0" x2="-1.27" y2="-1.905" width="0.254" layer="94"/>
<text x="-2.54" y="-2.54" size="1.778" layer="96" rot="R90">&gt;VALUE</text>
<pin name="VCC" x="0" y="-2.54" visible="off" length="short" direction="sup" rot="R90"/>
</symbol>
<symbol name="+3V3" urn="urn:adsk.eagle:symbol:26950/1" library_version="1">
<wire x1="1.27" y1="-1.905" x2="0" y2="0" width="0.254" layer="94"/>
<wire x1="0" y1="0" x2="-1.27" y2="-1.905" width="0.254" layer="94"/>
<text x="-2.54" y="-5.08" size="1.778" layer="96" rot="R90">&gt;VALUE</text>
<pin name="+3V3" x="0" y="-2.54" visible="off" length="short" direction="sup" rot="R90"/>
</symbol>
<symbol name="PE" urn="urn:adsk.eagle:symbol:26941/1" library_version="1">
<wire x1="-1.905" y1="0" x2="1.905" y2="0" width="0.254" layer="94"/>
<wire x1="-1.0922" y1="-0.508" x2="1.0922" y2="-0.508" width="0.254" layer="94"/>
<wire x1="-0.6223" y1="-1.016" x2="0.6223" y2="-1.016" width="0.254" layer="94"/>
<wire x1="-0.3048" y1="-1.524" x2="0.3302" y2="-1.524" width="0.254" layer="94"/>
<text x="-2.54" y="-2.54" size="1.778" layer="96" rot="R90">&gt;VALUE</text>
<pin name="PE" x="0" y="2.54" visible="off" length="short" direction="sup" rot="R270"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="GND" urn="urn:adsk.eagle:component:26954/1" prefix="GND" library_version="1">
<description>&lt;b&gt;SUPPLY SYMBOL&lt;/b&gt;</description>
<gates>
<gate name="1" symbol="GND" x="0" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="+12V" urn="urn:adsk.eagle:component:26959/1" prefix="P+" library_version="1">
<description>&lt;b&gt;SUPPLY SYMBOL&lt;/b&gt;</description>
<gates>
<gate name="1" symbol="+12V" x="0" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="VCC" urn="urn:adsk.eagle:component:26957/1" prefix="P+" library_version="1">
<description>&lt;b&gt;SUPPLY SYMBOL&lt;/b&gt;</description>
<gates>
<gate name="VCC" symbol="VCC" x="0" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="+3V3" urn="urn:adsk.eagle:component:26981/1" prefix="+3V3" library_version="1">
<description>&lt;b&gt;SUPPLY SYMBOL&lt;/b&gt;</description>
<gates>
<gate name="G$1" symbol="+3V3" x="0" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="PE" urn="urn:adsk.eagle:component:26969/1" prefix="PE" library_version="1">
<description>&lt;b&gt;SUPPLY SYMBOL&lt;/b&gt;</description>
<gates>
<gate name="M" symbol="PE" x="0" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="trafo" urn="urn:adsk.eagle:library:394">
<description>&lt;b&gt;Transformers&lt;/b&gt;&lt;p&gt;
Based on the following previous libraries:
&lt;ul&gt;
&lt;li&gt;trafo-b.lbr: transformers from BLOCK
&lt;li&gt;trafo-e.lbr: transformers from ERA
&lt;li&gt;trafo-r.lbr: ring cores
&lt;/ul&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="EI30-1" urn="urn:adsk.eagle:footprint:28248/1" library_version="1">
<description>&lt;b&gt;TRANSFORMER&lt;/b&gt;&lt;p&gt;
1 primary winding, 1 secondary winding</description>
<wire x1="-3.175" y1="3.81" x2="-3.175" y2="-3.81" width="0.254" layer="21"/>
<wire x1="-1.27" y1="3.81" x2="-1.27" y2="-3.81" width="0.254" layer="21"/>
<wire x1="-1.27" y1="-3.81" x2="-3.175" y2="-3.81" width="0.254" layer="21"/>
<wire x1="0" y1="3.81" x2="0" y2="-3.81" width="0.254" layer="21"/>
<wire x1="1.27" y1="2.54" x2="1.27" y2="-2.54" width="0.254" layer="21"/>
<wire x1="1.27" y1="-2.54" x2="3.175" y2="-2.54" width="0.254" layer="21"/>
<wire x1="1.27" y1="2.54" x2="3.175" y2="2.54" width="0.254" layer="21"/>
<wire x1="3.175" y1="-2.54" x2="3.175" y2="2.54" width="0.254" layer="21"/>
<wire x1="-3.175" y1="3.81" x2="-1.27" y2="3.81" width="0.254" layer="21"/>
<wire x1="-5.08" y1="3.81" x2="-3.175" y2="3.81" width="0.254" layer="21"/>
<wire x1="-3.175" y1="-3.81" x2="-5.08" y2="-3.81" width="0.254" layer="21"/>
<wire x1="3.175" y1="-2.54" x2="5.08" y2="-2.54" width="0.254" layer="21"/>
<wire x1="3.175" y1="2.54" x2="5.08" y2="2.54" width="0.254" layer="21"/>
<wire x1="-13.716" y1="-14.986" x2="-12.446" y2="-16.256" width="0.254" layer="21" curve="90"/>
<wire x1="12.446" y1="16.256" x2="13.716" y2="14.986" width="0.254" layer="21" curve="-90"/>
<wire x1="12.446" y1="-16.256" x2="13.716" y2="-14.986" width="0.254" layer="21" curve="90"/>
<wire x1="13.716" y1="14.986" x2="13.716" y2="-14.986" width="0.254" layer="21"/>
<wire x1="-13.716" y1="14.986" x2="-12.446" y2="16.256" width="0.254" layer="21" curve="-90"/>
<wire x1="-13.716" y1="-14.986" x2="-13.716" y2="14.986" width="0.254" layer="21"/>
<wire x1="8.763" y1="12.192" x2="9.398" y2="11.557" width="0.254" layer="51" curve="-90"/>
<wire x1="8.763" y1="-12.192" x2="9.398" y2="-11.557" width="0.254" layer="51" curve="90"/>
<wire x1="-9.398" y1="11.557" x2="-8.763" y2="12.192" width="0.254" layer="51" curve="-90"/>
<wire x1="-9.398" y1="-11.557" x2="-8.763" y2="-12.192" width="0.254" layer="51" curve="90"/>
<wire x1="12.446" y1="-16.256" x2="-12.446" y2="-16.256" width="0.254" layer="21"/>
<wire x1="-12.446" y1="16.256" x2="12.446" y2="16.256" width="0.254" layer="21"/>
<wire x1="-9.398" y1="-11.557" x2="-9.398" y2="-7.874" width="0.254" layer="51"/>
<wire x1="-8.763" y1="-12.192" x2="8.763" y2="-12.192" width="0.254" layer="21"/>
<wire x1="8.763" y1="12.192" x2="-8.763" y2="12.192" width="0.254" layer="21"/>
<wire x1="9.398" y1="-8.001" x2="9.398" y2="-11.557" width="0.254" layer="51"/>
<wire x1="-9.398" y1="-7.874" x2="-9.398" y2="7.874" width="0.254" layer="21"/>
<wire x1="9.398" y1="-8.001" x2="9.398" y2="-7.112" width="0.254" layer="21"/>
<wire x1="9.398" y1="-2.921" x2="9.398" y2="-7.112" width="0.254" layer="51"/>
<wire x1="9.398" y1="-2.921" x2="9.398" y2="2.921" width="0.254" layer="21"/>
<wire x1="9.398" y1="7.112" x2="9.398" y2="2.921" width="0.254" layer="51"/>
<wire x1="9.398" y1="7.112" x2="9.398" y2="7.874" width="0.254" layer="21"/>
<wire x1="9.398" y1="11.557" x2="9.398" y2="7.874" width="0.254" layer="51"/>
<wire x1="-9.398" y1="7.874" x2="-9.398" y2="11.557" width="0.254" layer="51"/>
<circle x="-3.937" y="-3.048" radius="0.254" width="0.254" layer="21"/>
<circle x="3.937" y="-1.778" radius="0.254" width="0.254" layer="21"/>
<circle x="-5.715" y="3.81" radius="0.635" width="0.254" layer="21"/>
<circle x="-5.715" y="-3.81" radius="0.635" width="0.254" layer="21"/>
<circle x="5.715" y="-2.54" radius="0.635" width="0.254" layer="21"/>
<circle x="5.715" y="2.54" radius="0.635" width="0.254" layer="21"/>
<pad name="1" x="-10.0076" y="10.0076" drill="1.1176" diameter="3.81" shape="octagon"/>
<pad name="5" x="-10.0076" y="-10.0076" drill="1.1176" diameter="3.81" shape="octagon"/>
<pad name="6" x="10.0076" y="-10.0076" drill="1.1176" diameter="3.81" shape="octagon"/>
<pad name="7" x="10.0076" y="-5.0038" drill="1.1176" diameter="3.81" shape="octagon"/>
<pad name="9" x="10.0076" y="5.0038" drill="1.1176" diameter="3.81" shape="octagon"/>
<pad name="10" x="10.0076" y="10.0076" drill="1.1176" diameter="3.81" shape="octagon"/>
<text x="-12.7" y="16.891" size="1.778" layer="25" ratio="10">&gt;NAME</text>
<text x="-3.81" y="16.891" size="1.778" layer="27" ratio="10">&gt;VALUE</text>
<text x="-6.731" y="-0.889" size="1.778" layer="21" ratio="10">P1</text>
<text x="6.477" y="-1.27" size="1.778" layer="21" ratio="10" rot="R90">S1</text>
<text x="-6.35" y="-10.16" size="1.778" layer="21" ratio="10">EI30-1</text>
<text x="-12.065" y="12.7" size="1.27" layer="21" ratio="10">1</text>
<text x="-12.065" y="-13.97" size="1.27" layer="21" ratio="10">5</text>
<text x="10.16" y="12.7" size="1.27" layer="21" ratio="10">10</text>
<text x="10.795" y="-13.97" size="1.27" layer="21" ratio="10">6</text>
<rectangle x1="-3.175" y1="-3.81" x2="-1.27" y2="3.81" layer="21"/>
<rectangle x1="1.27" y1="-2.54" x2="3.175" y2="2.54" layer="21"/>
</package>
</packages>
<packages3d>
<package3d name="EI30-1" urn="urn:adsk.eagle:package:28333/1" type="box" library_version="1">
<description>TRANSFORMER
1 primary winding, 1 secondary winding</description>
<packageinstances>
<packageinstance name="EI30-1"/>
</packageinstances>
</package3d>
</packages3d>
<symbols>
<symbol name="T1P1S" urn="urn:adsk.eagle:symbol:28193/1" library_version="1">
<wire x1="1.27" y1="5.207" x2="1.27" y2="-7.747" width="0.254" layer="94"/>
<circle x="-2.921" y="-8.636" radius="0.127" width="0.3048" layer="94"/>
<circle x="5.461" y="-5.842" radius="0.127" width="0.3048" layer="94"/>
<text x="-2.54" y="7.366" size="1.778" layer="95">&gt;NAME</text>
<text x="-2.54" y="-10.795" size="1.778" layer="96">&gt;VALUE</text>
<text x="-3.175" y="-2.54" size="1.27" layer="94" rot="R90">PRI</text>
<text x="6.985" y="-2.54" size="1.27" layer="94" rot="R90">SEC</text>
<rectangle x1="-2.286" y1="-7.62" x2="0.254" y2="5.08" layer="94"/>
<rectangle x1="2.286" y1="-5.08" x2="4.826" y2="2.54" layer="94"/>
<pin name="P1" x="-5.08" y="5.08" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="P2" x="-5.08" y="-7.62" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="S3" x="7.62" y="-5.08" visible="pad" length="middle" direction="pas" swaplevel="2" rot="R180"/>
<pin name="S4" x="7.62" y="2.54" visible="pad" length="middle" direction="pas" swaplevel="2" rot="R180"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="EI30-1" urn="urn:adsk.eagle:component:28412/1" prefix="TR" uservalue="yes" library_version="1">
<description>&lt;b&gt;TRANSFORMER&lt;/b&gt;&lt;p&gt;
1 primary winding, 1 secondary winding&lt;p&gt;
ERA</description>
<gates>
<gate name="1" symbol="T1P1S" x="0" y="0"/>
</gates>
<devices>
<device name="" package="EI30-1">
<connects>
<connect gate="1" pin="P1" pad="1"/>
<connect gate="1" pin="P2" pad="5"/>
<connect gate="1" pin="S3" pad="7"/>
<connect gate="1" pin="S4" pad="9"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:28333/1"/>
</package3dinstances>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="diode" urn="urn:adsk.eagle:library:210">
<description>&lt;b&gt;Diodes&lt;/b&gt;&lt;p&gt;
Based on the following sources:
&lt;ul&gt;
&lt;li&gt;Motorola : www.onsemi.com
&lt;li&gt;Fairchild : www.fairchildsemi.com
&lt;li&gt;Philips : www.semiconductors.com
&lt;li&gt;Vishay : www.vishay.de
&lt;/ul&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="DO41-10" urn="urn:adsk.eagle:footprint:43094/1" library_version="7">
<description>&lt;B&gt;DIODE&lt;/B&gt;&lt;p&gt;
diameter 2.54 mm, horizontal, grid 10.16 mm</description>
<wire x1="2.032" y1="-1.27" x2="-2.032" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="2.032" y1="-1.27" x2="2.032" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-2.032" y1="1.27" x2="2.032" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-2.032" y1="1.27" x2="-2.032" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="5.08" y1="0" x2="4.064" y2="0" width="0.762" layer="51"/>
<wire x1="-5.08" y1="0" x2="-4.064" y2="0" width="0.762" layer="51"/>
<wire x1="-0.635" y1="0" x2="0" y2="0" width="0.1524" layer="21"/>
<wire x1="1.016" y1="0.635" x2="1.016" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="1.016" y1="-0.635" x2="0" y2="0" width="0.1524" layer="21"/>
<wire x1="0" y1="0" x2="1.524" y2="0" width="0.1524" layer="21"/>
<wire x1="0" y1="0" x2="1.016" y2="0.635" width="0.1524" layer="21"/>
<wire x1="0" y1="0.635" x2="0" y2="0" width="0.1524" layer="21"/>
<wire x1="0" y1="0" x2="0" y2="-0.635" width="0.1524" layer="21"/>
<pad name="A" x="5.08" y="0" drill="1.1176"/>
<pad name="C" x="-5.08" y="0" drill="1.1176"/>
<text x="-2.032" y="1.651" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-2.032" y="-2.794" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="-1.651" y1="-1.27" x2="-1.143" y2="1.27" layer="21"/>
<rectangle x1="2.032" y1="-0.381" x2="3.937" y2="0.381" layer="21"/>
<rectangle x1="-3.937" y1="-0.381" x2="-2.032" y2="0.381" layer="21"/>
</package>
</packages>
<packages3d>
<package3d name="DO41-10" urn="urn:adsk.eagle:package:43336/1" type="box" library_version="7">
<description>DIODE
diameter 2.54 mm, horizontal, grid 10.16 mm</description>
<packageinstances>
<packageinstance name="DO41-10"/>
</packageinstances>
</package3d>
</packages3d>
<symbols>
<symbol name="D" urn="urn:adsk.eagle:symbol:43091/2" library_version="7">
<wire x1="-1.27" y1="-1.27" x2="1.27" y2="0" width="0.254" layer="94"/>
<wire x1="1.27" y1="0" x2="-1.27" y2="1.27" width="0.254" layer="94"/>
<wire x1="1.27" y1="1.27" x2="1.27" y2="0" width="0.254" layer="94"/>
<wire x1="-1.27" y1="1.27" x2="-1.27" y2="0" width="0.254" layer="94"/>
<wire x1="-1.27" y1="0" x2="-1.27" y2="-1.27" width="0.254" layer="94"/>
<wire x1="1.27" y1="0" x2="1.27" y2="-1.27" width="0.254" layer="94"/>
<wire x1="-1.27" y1="0" x2="-2.54" y2="0" width="0.254" layer="94"/>
<wire x1="2.54" y1="0" x2="1.27" y2="0" width="0.254" layer="94"/>
<text x="2.54" y="0.4826" size="1.778" layer="95">&gt;NAME</text>
<text x="2.54" y="-2.3114" size="1.778" layer="96">&gt;VALUE</text>
<text x="-2.54" y="0" size="0.4064" layer="99" align="center">SpiceOrder 1</text>
<text x="2.54" y="0" size="0.4064" layer="99" align="center">SpiceOrder 2</text>
<pin name="A" x="-2.54" y="0" visible="off" length="point" direction="pas"/>
<pin name="C" x="2.54" y="0" visible="off" length="point" direction="pas" rot="R180"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="1N4004" urn="urn:adsk.eagle:component:43462/2" prefix="D" library_version="7">
<description>&lt;B&gt;DIODE&lt;/B&gt;&lt;p&gt;
general purpose rectifier, 1 A</description>
<gates>
<gate name="1" symbol="D" x="0" y="0"/>
</gates>
<devices>
<device name="" package="DO41-10">
<connects>
<connect gate="1" pin="A" pad="A"/>
<connect gate="1" pin="C" pad="C"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:43336/1"/>
</package3dinstances>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="con-amp-quick" urn="urn:adsk.eagle:library:125">
<description>&lt;b&gt;AMP Connectors, Type QUICK&lt;/b&gt;&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="02P" urn="urn:adsk.eagle:footprint:5910/1" library_version="1">
<description>&lt;b&gt;AMP QUICK CONNECTOR&lt;/b&gt;</description>
<wire x1="-2.286" y1="2.54" x2="-2.286" y2="1.778" width="0.1524" layer="21"/>
<wire x1="-2.286" y1="1.778" x2="-0.381" y2="1.778" width="0.1524" layer="21"/>
<wire x1="-0.381" y1="1.778" x2="-0.254" y2="1.651" width="0.1524" layer="21"/>
<wire x1="-0.254" y1="1.651" x2="0" y2="1.397" width="0.1524" layer="21"/>
<wire x1="0" y1="1.397" x2="0.254" y2="1.651" width="0.1524" layer="21"/>
<wire x1="0.254" y1="1.651" x2="0.381" y2="1.778" width="0.1524" layer="21"/>
<wire x1="-0.254" y1="-2.159" x2="-0.254" y2="-2.286" width="0.1524" layer="21"/>
<wire x1="-0.254" y1="-2.286" x2="-2.286" y2="-2.286" width="0.1524" layer="21"/>
<wire x1="0.254" y1="-2.159" x2="-0.254" y2="-2.159" width="0.1524" layer="21"/>
<wire x1="0.254" y1="-2.159" x2="0.254" y2="-2.286" width="0.1524" layer="21"/>
<wire x1="2.286" y1="-2.286" x2="0.254" y2="-2.286" width="0.1524" layer="21"/>
<wire x1="0.381" y1="1.778" x2="2.286" y2="1.778" width="0.1524" layer="21"/>
<wire x1="2.286" y1="1.778" x2="2.286" y2="2.54" width="0.1524" layer="21"/>
<wire x1="-2.286" y1="1.778" x2="-2.286" y2="-1.524" width="0.1524" layer="21"/>
<wire x1="-2.286" y1="-1.778" x2="-2.286" y2="-2.286" width="0.1524" layer="21"/>
<wire x1="-2.286" y1="-1.778" x2="-1.905" y2="-1.778" width="0.1524" layer="21"/>
<wire x1="2.286" y1="-1.778" x2="2.286" y2="-2.286" width="0.1524" layer="21"/>
<wire x1="2.286" y1="-1.778" x2="1.905" y2="-1.778" width="0.1524" layer="21"/>
<wire x1="2.286" y1="1.778" x2="2.286" y2="-1.524" width="0.1524" layer="21"/>
<wire x1="1.905" y1="-1.778" x2="1.905" y2="-1.524" width="0.1524" layer="21"/>
<wire x1="0.635" y1="-1.27" x2="1.905" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="-1.778" x2="-0.635" y2="-1.524" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="-1.778" x2="-1.905" y2="-1.524" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="-1.27" x2="-0.635" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="-1.778" x2="0.635" y2="-1.778" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="-1.524" x2="-0.635" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="0.635" y1="-1.778" x2="0.635" y2="-1.524" width="0.1524" layer="21"/>
<wire x1="0.635" y1="-1.524" x2="0.635" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="0.254" y1="-1.524" x2="0.635" y2="-1.524" width="0.1524" layer="21"/>
<wire x1="0.254" y1="-1.524" x2="0.254" y2="1.651" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="-1.524" x2="-0.254" y2="-1.524" width="0.1524" layer="21"/>
<wire x1="-0.254" y1="-1.524" x2="0.254" y2="-1.524" width="0.1524" layer="21"/>
<wire x1="-0.254" y1="-1.524" x2="-0.254" y2="1.651" width="0.1524" layer="21"/>
<wire x1="1.905" y1="-1.524" x2="2.286" y2="-1.524" width="0.1524" layer="21"/>
<wire x1="1.905" y1="-1.524" x2="1.905" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="2.286" y1="-1.524" x2="2.286" y2="-1.778" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="-1.524" x2="-2.286" y2="-1.524" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="-1.524" x2="-1.905" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-2.286" y1="-1.524" x2="-2.286" y2="-1.778" width="0.1524" layer="21"/>
<wire x1="-2.286" y1="2.54" x2="-1.524" y2="2.54" width="0.1524" layer="21"/>
<wire x1="-1.524" y1="2.54" x2="-1.524" y2="2.667" width="0.1524" layer="21"/>
<wire x1="-1.016" y1="2.54" x2="-1.016" y2="2.667" width="0.1524" layer="21"/>
<wire x1="-1.016" y1="2.667" x2="-1.524" y2="2.667" width="0.1524" layer="21"/>
<wire x1="1.016" y1="2.54" x2="1.016" y2="2.667" width="0.1524" layer="21"/>
<wire x1="1.524" y1="2.54" x2="2.286" y2="2.54" width="0.1524" layer="21"/>
<wire x1="1.524" y1="2.54" x2="1.524" y2="2.667" width="0.1524" layer="21"/>
<wire x1="1.524" y1="2.667" x2="1.016" y2="2.667" width="0.1524" layer="21"/>
<wire x1="0.254" y1="2.413" x2="0.254" y2="2.54" width="0.1524" layer="21"/>
<wire x1="0.254" y1="2.54" x2="1.016" y2="2.54" width="0.1524" layer="21"/>
<wire x1="0.254" y1="2.413" x2="-0.254" y2="2.413" width="0.1524" layer="21"/>
<wire x1="-0.254" y1="2.413" x2="-0.254" y2="2.54" width="0.1524" layer="21"/>
<wire x1="-0.254" y1="2.54" x2="-1.016" y2="2.54" width="0.1524" layer="21"/>
<pad name="1" x="-1.27" y="0" drill="0.9144" shape="long" rot="R90"/>
<pad name="2" x="1.27" y="0" drill="0.9144" shape="long" rot="R90"/>
<text x="-0.762" y="0.9398" size="0.9906" layer="21" ratio="12" rot="R90">1</text>
<text x="-2.286" y="2.9464" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-2.286" y="-3.8354" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="-1.6002" y1="-0.3302" x2="-0.9398" y2="0.3302" layer="51"/>
<rectangle x1="0.9398" y1="-0.3302" x2="1.6002" y2="0.3302" layer="51"/>
</package>
</packages>
<packages3d>
<package3d name="02P" urn="urn:adsk.eagle:package:5956/1" type="box" library_version="1">
<description>AMP QUICK CONNECTOR</description>
<packageinstances>
<packageinstance name="02P"/>
</packageinstances>
</package3d>
</packages3d>
<symbols>
<symbol name="M02" urn="urn:adsk.eagle:symbol:5909/1" library_version="1">
<wire x1="3.81" y1="-2.54" x2="-2.54" y2="-2.54" width="0.4064" layer="94"/>
<wire x1="1.27" y1="2.54" x2="2.54" y2="2.54" width="0.6096" layer="94"/>
<wire x1="1.27" y1="0" x2="2.54" y2="0" width="0.6096" layer="94"/>
<wire x1="-2.54" y1="5.08" x2="-2.54" y2="-2.54" width="0.4064" layer="94"/>
<wire x1="3.81" y1="-2.54" x2="3.81" y2="5.08" width="0.4064" layer="94"/>
<wire x1="-2.54" y1="5.08" x2="3.81" y2="5.08" width="0.4064" layer="94"/>
<text x="-2.54" y="-5.08" size="1.778" layer="96">&gt;VALUE</text>
<text x="-2.54" y="5.842" size="1.778" layer="95">&gt;NAME</text>
<pin name="1" x="7.62" y="0" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="2" x="7.62" y="2.54" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="M02" urn="urn:adsk.eagle:component:5982/1" prefix="SL" uservalue="yes" library_version="1">
<description>&lt;b&gt;AMP QUICK CONNECTOR&lt;/b&gt;</description>
<gates>
<gate name="G$1" symbol="M02" x="-2.54" y="0"/>
</gates>
<devices>
<device name="" package="02P">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:5956/1"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="MF" value="" constant="no"/>
<attribute name="MPN" value="" constant="no"/>
<attribute name="OC_FARNELL" value="unknown" constant="no"/>
<attribute name="OC_NEWARK" value="unknown" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="K2" library="relay" library_urn="urn:adsk.eagle:library:339" deviceset="G2R" device="" package3d_urn="urn:adsk.eagle:package:24323/1"/>
<part name="K3" library="relay" library_urn="urn:adsk.eagle:library:339" deviceset="G2R" device="" package3d_urn="urn:adsk.eagle:package:24323/1"/>
<part name="SUPPLY1" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="+12V" device=""/>
<part name="GND1" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="GND" device=""/>
<part name="12/230V" library="trafo" library_urn="urn:adsk.eagle:library:394" deviceset="EI30-1" device="" package3d_urn="urn:adsk.eagle:package:28333/1" value="12/230V"/>
<part name="230/12V" library="trafo" library_urn="urn:adsk.eagle:library:394" deviceset="EI30-1" device="" package3d_urn="urn:adsk.eagle:package:28333/1" value="230/12V"/>
<part name="D1" library="diode" library_urn="urn:adsk.eagle:library:210" deviceset="1N4004" device="" package3d_urn="urn:adsk.eagle:package:43336/1"/>
<part name="D2" library="diode" library_urn="urn:adsk.eagle:library:210" deviceset="1N4004" device="" package3d_urn="urn:adsk.eagle:package:43336/1"/>
<part name="P+1" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="+12V" device=""/>
<part name="K1" library="relay" library_urn="urn:adsk.eagle:library:339" deviceset="G2R" device="" package3d_urn="urn:adsk.eagle:package:24323/1"/>
<part name="GND2" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="GND" device=""/>
<part name="P+2" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="VCC" device="" value="230V"/>
<part name="+3V1" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="+3V3" device=""/>
<part name="+3V2" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="+3V3" device=""/>
<part name="P+3" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="VCC" device="" value="230V"/>
<part name="GND3" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="GND" device=""/>
<part name="P+4" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="VCC" device="" value="230V"/>
<part name="PE1" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="PE" device=""/>
<part name="PE2" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="PE" device=""/>
<part name="PE3" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="PE" device=""/>
<part name="SL1" library="con-amp-quick" library_urn="urn:adsk.eagle:library:125" deviceset="M02" device="" package3d_urn="urn:adsk.eagle:package:5956/1"/>
<part name="SUPPLY2" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="+12V" device=""/>
<part name="GND4" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="GND" device=""/>
<part name="12/230V1" library="trafo" library_urn="urn:adsk.eagle:library:394" deviceset="EI30-1" device="" package3d_urn="urn:adsk.eagle:package:28333/1" value="12/230V"/>
<part name="230/12V1" library="trafo" library_urn="urn:adsk.eagle:library:394" deviceset="EI30-1" device="" package3d_urn="urn:adsk.eagle:package:28333/1" value="230/12V"/>
<part name="D3" library="diode" library_urn="urn:adsk.eagle:library:210" deviceset="1N4004" device="" package3d_urn="urn:adsk.eagle:package:43336/1"/>
<part name="D4" library="diode" library_urn="urn:adsk.eagle:library:210" deviceset="1N4004" device="" package3d_urn="urn:adsk.eagle:package:43336/1"/>
<part name="P+5" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="+12V" device=""/>
<part name="GND5" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="GND" device=""/>
<part name="P+6" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="VCC" device="" value="230V"/>
<part name="+3V3" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="+3V3" device=""/>
<part name="+3V4" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="+3V3" device=""/>
<part name="P+7" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="VCC" device="" value="230V"/>
<part name="GND6" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="GND" device=""/>
<part name="P+8" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="VCC" device="" value="230V"/>
<part name="PE4" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="PE" device=""/>
<part name="PE5" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="PE" device=""/>
<part name="PE6" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="PE" device=""/>
<part name="SL2" library="con-amp-quick" library_urn="urn:adsk.eagle:library:125" deviceset="M02" device="" package3d_urn="urn:adsk.eagle:package:5956/1"/>
<part name="K7" library="relay" library_urn="urn:adsk.eagle:library:339" deviceset="G2R" device="" package3d_urn="urn:adsk.eagle:package:24323/1"/>
<part name="K8" library="relay" library_urn="urn:adsk.eagle:library:339" deviceset="G2R" device="" package3d_urn="urn:adsk.eagle:package:24323/1"/>
<part name="K5" library="relay" library_urn="urn:adsk.eagle:library:339" deviceset="G2R" device="" package3d_urn="urn:adsk.eagle:package:24323/1"/>
<part name="PE7" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="PE" device=""/>
<part name="PE8" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="PE" device=""/>
<part name="SUPPLY3" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="+12V" device=""/>
<part name="GND7" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="GND" device=""/>
<part name="12/230V2" library="trafo" library_urn="urn:adsk.eagle:library:394" deviceset="EI30-1" device="" package3d_urn="urn:adsk.eagle:package:28333/1" value="12/230V"/>
<part name="230/12V2" library="trafo" library_urn="urn:adsk.eagle:library:394" deviceset="EI30-1" device="" package3d_urn="urn:adsk.eagle:package:28333/1" value="230/12V"/>
<part name="D5" library="diode" library_urn="urn:adsk.eagle:library:210" deviceset="1N4004" device="" package3d_urn="urn:adsk.eagle:package:43336/1"/>
<part name="D6" library="diode" library_urn="urn:adsk.eagle:library:210" deviceset="1N4004" device="" package3d_urn="urn:adsk.eagle:package:43336/1"/>
<part name="P+9" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="+12V" device=""/>
<part name="GND8" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="GND" device=""/>
<part name="P+10" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="VCC" device="" value="230V"/>
<part name="+3V5" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="+3V3" device=""/>
<part name="+3V6" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="+3V3" device=""/>
<part name="P+11" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="VCC" device="" value="230V"/>
<part name="GND9" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="GND" device=""/>
<part name="P+12" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="VCC" device="" value="230V"/>
<part name="PE9" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="PE" device=""/>
<part name="PE10" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="PE" device=""/>
<part name="PE11" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="PE" device=""/>
<part name="SL3" library="con-amp-quick" library_urn="urn:adsk.eagle:library:125" deviceset="M02" device="" package3d_urn="urn:adsk.eagle:package:5956/1"/>
<part name="K4" library="relay" library_urn="urn:adsk.eagle:library:339" deviceset="G2R" device="" package3d_urn="urn:adsk.eagle:package:24323/1"/>
<part name="PE12" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="PE" device=""/>
<part name="K6" library="relay" library_urn="urn:adsk.eagle:library:339" deviceset="G2R" device="" package3d_urn="urn:adsk.eagle:package:24323/1"/>
<part name="K9" library="relay" library_urn="urn:adsk.eagle:library:339" deviceset="G2R" device="" package3d_urn="urn:adsk.eagle:package:24323/1"/>
</parts>
<sheets>
<sheet>
<plain>
</plain>
<instances>
<instance part="K2" gate="1" x="11.43" y="49.53" smashed="yes" rot="R90">
<attribute name="VALUE" x="8.509" y="50.8" size="1.778" layer="96" rot="R90"/>
<attribute name="PART" x="6.35" y="50.8" size="1.778" layer="95" rot="R90"/>
</instance>
<instance part="K2" gate="2" x="8.89" y="62.23" smashed="yes" rot="R270">
<attribute name="PART" x="8.89" y="59.69" size="1.778" layer="95" rot="R270"/>
</instance>
<instance part="K3" gate="1" x="34.29" y="36.83" smashed="yes" rot="R90">
<attribute name="VALUE" x="31.369" y="38.1" size="1.778" layer="96" rot="R90"/>
<attribute name="PART" x="29.21" y="38.1" size="1.778" layer="95" rot="R90"/>
</instance>
<instance part="K3" gate="2" x="34.29" y="49.53" smashed="yes" rot="R90">
<attribute name="PART" x="34.29" y="52.07" size="1.778" layer="95" rot="R90"/>
</instance>
<instance part="SUPPLY1" gate="+12V" x="-8.89" y="62.23" smashed="yes" rot="R90">
<attribute name="VALUE" x="-12.065" y="59.69" size="1.778" layer="96" rot="R90"/>
</instance>
<instance part="GND1" gate="1" x="100.33" y="50.8" smashed="yes">
<attribute name="VALUE" x="97.79" y="48.26" size="1.778" layer="96"/>
</instance>
<instance part="12/230V" gate="1" x="64.77" y="10.16" smashed="yes" rot="R180">
<attribute name="NAME" x="67.31" y="2.794" size="1.778" layer="95" rot="R180"/>
<attribute name="VALUE" x="67.31" y="20.955" size="1.778" layer="96" rot="R180"/>
</instance>
<instance part="230/12V" gate="1" x="60.96" y="58.42" smashed="yes">
<attribute name="NAME" x="58.42" y="65.786" size="1.778" layer="95"/>
<attribute name="VALUE" x="58.42" y="47.625" size="1.778" layer="96"/>
</instance>
<instance part="D1" gate="1" x="77.47" y="72.39" smashed="yes">
<attribute name="NAME" x="80.01" y="72.8726" size="1.778" layer="95"/>
<attribute name="VALUE" x="80.01" y="70.0786" size="1.778" layer="96"/>
</instance>
<instance part="D2" gate="1" x="77.47" y="60.96" smashed="yes">
<attribute name="NAME" x="80.01" y="61.4426" size="1.778" layer="95"/>
<attribute name="VALUE" x="80.01" y="58.6486" size="1.778" layer="96"/>
</instance>
<instance part="P+1" gate="1" x="101.6" y="60.96" smashed="yes" rot="R270">
<attribute name="VALUE" x="96.52" y="63.5" size="1.778" layer="96"/>
</instance>
<instance part="K1" gate="1" x="33.02" y="3.81" smashed="yes" rot="R90">
<attribute name="VALUE" x="30.099" y="5.08" size="1.778" layer="96" rot="R90"/>
<attribute name="PART" x="27.94" y="5.08" size="1.778" layer="95" rot="R90"/>
</instance>
<instance part="K1" gate="2" x="35.56" y="20.32" smashed="yes" rot="R90">
<attribute name="PART" x="35.56" y="22.86" size="1.778" layer="95" rot="R90"/>
</instance>
<instance part="GND2" gate="1" x="71.12" y="1.27" smashed="yes">
<attribute name="VALUE" x="68.58" y="-1.27" size="1.778" layer="96"/>
</instance>
<instance part="P+2" gate="VCC" x="41.91" y="49.53" smashed="yes" rot="R270">
<attribute name="VALUE" x="39.37" y="52.07" size="1.778" layer="96"/>
</instance>
<instance part="+3V1" gate="G$1" x="19.05" y="52.07" smashed="yes">
<attribute name="VALUE" x="16.51" y="46.99" size="1.778" layer="96" rot="R90"/>
</instance>
<instance part="+3V2" gate="G$1" x="43.18" y="39.37" smashed="yes">
<attribute name="VALUE" x="40.64" y="34.29" size="1.778" layer="96" rot="R90"/>
</instance>
<instance part="P+3" gate="VCC" x="30.48" y="27.94" smashed="yes">
<attribute name="VALUE" x="27.94" y="25.4" size="1.778" layer="96" rot="R90"/>
</instance>
<instance part="GND3" gate="1" x="-10.16" y="54.61" smashed="yes">
<attribute name="VALUE" x="-12.7" y="52.07" size="1.778" layer="96"/>
</instance>
<instance part="P+4" gate="VCC" x="102.87" y="31.75" smashed="yes" rot="R270">
<attribute name="VALUE" x="100.33" y="34.29" size="1.778" layer="96"/>
</instance>
<instance part="PE1" gate="M" x="54.61" y="-3.81" smashed="yes">
<attribute name="VALUE" x="52.07" y="-6.35" size="1.778" layer="96" rot="R90"/>
</instance>
<instance part="PE2" gate="M" x="101.6" y="20.32" smashed="yes">
<attribute name="VALUE" x="99.06" y="17.78" size="1.778" layer="96" rot="R90"/>
</instance>
<instance part="PE3" gate="M" x="55.88" y="43.18" smashed="yes">
<attribute name="VALUE" x="53.34" y="40.64" size="1.778" layer="96" rot="R90"/>
</instance>
<instance part="SL1" gate="G$1" x="-29.21" y="46.99" smashed="yes">
<attribute name="VALUE" x="-31.75" y="41.91" size="1.778" layer="96"/>
<attribute name="NAME" x="-31.75" y="52.832" size="1.778" layer="95"/>
</instance>
<instance part="SUPPLY2" gate="+12V" x="-5.08" y="-116.84" smashed="yes" rot="R90">
<attribute name="VALUE" x="-8.255" y="-119.38" size="1.778" layer="96" rot="R90"/>
</instance>
<instance part="GND4" gate="1" x="104.14" y="-128.27" smashed="yes">
<attribute name="VALUE" x="101.6" y="-130.81" size="1.778" layer="96"/>
</instance>
<instance part="12/230V1" gate="1" x="68.58" y="-168.91" smashed="yes" rot="R180">
<attribute name="NAME" x="71.12" y="-176.276" size="1.778" layer="95" rot="R180"/>
<attribute name="VALUE" x="71.12" y="-158.115" size="1.778" layer="96" rot="R180"/>
</instance>
<instance part="230/12V1" gate="1" x="64.77" y="-120.65" smashed="yes">
<attribute name="NAME" x="62.23" y="-113.284" size="1.778" layer="95"/>
<attribute name="VALUE" x="62.23" y="-131.445" size="1.778" layer="96"/>
</instance>
<instance part="D3" gate="1" x="81.28" y="-106.68" smashed="yes">
<attribute name="NAME" x="83.82" y="-106.1974" size="1.778" layer="95"/>
<attribute name="VALUE" x="83.82" y="-108.9914" size="1.778" layer="96"/>
</instance>
<instance part="D4" gate="1" x="81.28" y="-118.11" smashed="yes">
<attribute name="NAME" x="83.82" y="-117.6274" size="1.778" layer="95"/>
<attribute name="VALUE" x="83.82" y="-120.4214" size="1.778" layer="96"/>
</instance>
<instance part="P+5" gate="1" x="105.41" y="-118.11" smashed="yes" rot="R270">
<attribute name="VALUE" x="100.33" y="-115.57" size="1.778" layer="96"/>
</instance>
<instance part="GND5" gate="1" x="74.93" y="-177.8" smashed="yes">
<attribute name="VALUE" x="72.39" y="-180.34" size="1.778" layer="96"/>
</instance>
<instance part="P+6" gate="VCC" x="45.72" y="-129.54" smashed="yes" rot="R270">
<attribute name="VALUE" x="43.18" y="-127" size="1.778" layer="96"/>
</instance>
<instance part="+3V3" gate="G$1" x="22.86" y="-127" smashed="yes">
<attribute name="VALUE" x="20.32" y="-132.08" size="1.778" layer="96" rot="R90"/>
</instance>
<instance part="+3V4" gate="G$1" x="46.99" y="-139.7" smashed="yes">
<attribute name="VALUE" x="44.45" y="-144.78" size="1.778" layer="96" rot="R90"/>
</instance>
<instance part="P+7" gate="VCC" x="34.29" y="-151.13" smashed="yes">
<attribute name="VALUE" x="31.75" y="-153.67" size="1.778" layer="96" rot="R90"/>
</instance>
<instance part="GND6" gate="1" x="-6.35" y="-124.46" smashed="yes">
<attribute name="VALUE" x="-8.89" y="-127" size="1.778" layer="96"/>
</instance>
<instance part="P+8" gate="VCC" x="106.68" y="-147.32" smashed="yes" rot="R270">
<attribute name="VALUE" x="104.14" y="-144.78" size="1.778" layer="96"/>
</instance>
<instance part="PE4" gate="M" x="58.42" y="-182.88" smashed="yes">
<attribute name="VALUE" x="55.88" y="-185.42" size="1.778" layer="96" rot="R90"/>
</instance>
<instance part="PE5" gate="M" x="105.41" y="-158.75" smashed="yes">
<attribute name="VALUE" x="102.87" y="-161.29" size="1.778" layer="96" rot="R90"/>
</instance>
<instance part="PE6" gate="M" x="59.69" y="-135.89" smashed="yes">
<attribute name="VALUE" x="57.15" y="-138.43" size="1.778" layer="96" rot="R90"/>
</instance>
<instance part="SL2" gate="G$1" x="-25.4" y="-132.08" smashed="yes">
<attribute name="VALUE" x="-27.94" y="-137.16" size="1.778" layer="96"/>
<attribute name="NAME" x="-27.94" y="-126.238" size="1.778" layer="95"/>
</instance>
<instance part="K7" gate="1" x="15.24" y="-129.54" smashed="yes" rot="R270">
<attribute name="VALUE" x="18.161" y="-130.81" size="1.778" layer="96" rot="R270"/>
<attribute name="PART" x="20.32" y="-130.81" size="1.778" layer="95" rot="R270"/>
</instance>
<instance part="K7" gate="2" x="12.7" y="-116.84" smashed="yes" rot="MR90">
<attribute name="PART" x="12.7" y="-114.3" size="1.778" layer="95" rot="MR90"/>
</instance>
<instance part="K8" gate="1" x="38.1" y="-142.24" smashed="yes" rot="R90">
<attribute name="VALUE" x="35.179" y="-140.97" size="1.778" layer="96" rot="R90"/>
<attribute name="PART" x="33.02" y="-140.97" size="1.778" layer="95" rot="R90"/>
</instance>
<instance part="K8" gate="2" x="38.1" y="-129.54" smashed="yes" rot="MR270">
<attribute name="PART" x="38.1" y="-132.08" size="1.778" layer="95" rot="MR270"/>
</instance>
<instance part="K5" gate="1" x="36.83" y="-175.26" smashed="yes" rot="R90">
<attribute name="VALUE" x="33.909" y="-173.99" size="1.778" layer="96" rot="R90"/>
<attribute name="PART" x="31.75" y="-173.99" size="1.778" layer="95" rot="R90"/>
</instance>
<instance part="K5" gate="2" x="39.37" y="-158.75" smashed="yes" rot="MR270">
<attribute name="PART" x="39.37" y="-161.29" size="1.778" layer="95" rot="MR270"/>
</instance>
<instance part="PE7" gate="M" x="39.37" y="1.27" smashed="yes">
<attribute name="VALUE" x="36.83" y="-1.27" size="1.778" layer="96" rot="R90"/>
</instance>
<instance part="PE8" gate="M" x="43.18" y="-177.8" smashed="yes">
<attribute name="VALUE" x="40.64" y="-180.34" size="1.778" layer="96" rot="R90"/>
</instance>
<instance part="SUPPLY3" gate="+12V" x="-8.89" y="-24.13" smashed="yes" rot="R90">
<attribute name="VALUE" x="-12.065" y="-26.67" size="1.778" layer="96" rot="R90"/>
</instance>
<instance part="GND7" gate="1" x="100.33" y="-35.56" smashed="yes">
<attribute name="VALUE" x="97.79" y="-38.1" size="1.778" layer="96"/>
</instance>
<instance part="12/230V2" gate="1" x="64.77" y="-76.2" smashed="yes" rot="R180">
<attribute name="NAME" x="67.31" y="-83.566" size="1.778" layer="95" rot="R180"/>
<attribute name="VALUE" x="67.31" y="-65.405" size="1.778" layer="96" rot="R180"/>
</instance>
<instance part="230/12V2" gate="1" x="60.96" y="-27.94" smashed="yes">
<attribute name="NAME" x="58.42" y="-20.574" size="1.778" layer="95"/>
<attribute name="VALUE" x="58.42" y="-38.735" size="1.778" layer="96"/>
</instance>
<instance part="D5" gate="1" x="77.47" y="-13.97" smashed="yes">
<attribute name="NAME" x="80.01" y="-13.4874" size="1.778" layer="95"/>
<attribute name="VALUE" x="80.01" y="-16.2814" size="1.778" layer="96"/>
</instance>
<instance part="D6" gate="1" x="77.47" y="-25.4" smashed="yes">
<attribute name="NAME" x="80.01" y="-24.9174" size="1.778" layer="95"/>
<attribute name="VALUE" x="80.01" y="-27.7114" size="1.778" layer="96"/>
</instance>
<instance part="P+9" gate="1" x="101.6" y="-25.4" smashed="yes" rot="R270">
<attribute name="VALUE" x="96.52" y="-22.86" size="1.778" layer="96"/>
</instance>
<instance part="GND8" gate="1" x="71.12" y="-85.09" smashed="yes">
<attribute name="VALUE" x="68.58" y="-87.63" size="1.778" layer="96"/>
</instance>
<instance part="P+10" gate="VCC" x="41.91" y="-36.83" smashed="yes" rot="R270">
<attribute name="VALUE" x="39.37" y="-34.29" size="1.778" layer="96"/>
</instance>
<instance part="+3V5" gate="G$1" x="19.05" y="-34.29" smashed="yes">
<attribute name="VALUE" x="16.51" y="-39.37" size="1.778" layer="96" rot="R90"/>
</instance>
<instance part="+3V6" gate="G$1" x="43.18" y="-46.99" smashed="yes">
<attribute name="VALUE" x="40.64" y="-52.07" size="1.778" layer="96" rot="R90"/>
</instance>
<instance part="P+11" gate="VCC" x="30.48" y="-58.42" smashed="yes">
<attribute name="VALUE" x="27.94" y="-60.96" size="1.778" layer="96" rot="R90"/>
</instance>
<instance part="GND9" gate="1" x="-10.16" y="-31.75" smashed="yes">
<attribute name="VALUE" x="-12.7" y="-34.29" size="1.778" layer="96"/>
</instance>
<instance part="P+12" gate="VCC" x="102.87" y="-54.61" smashed="yes" rot="R270">
<attribute name="VALUE" x="100.33" y="-52.07" size="1.778" layer="96"/>
</instance>
<instance part="PE9" gate="M" x="54.61" y="-90.17" smashed="yes">
<attribute name="VALUE" x="52.07" y="-92.71" size="1.778" layer="96" rot="R90"/>
</instance>
<instance part="PE10" gate="M" x="101.6" y="-66.04" smashed="yes">
<attribute name="VALUE" x="99.06" y="-68.58" size="1.778" layer="96" rot="R90"/>
</instance>
<instance part="PE11" gate="M" x="55.88" y="-43.18" smashed="yes">
<attribute name="VALUE" x="53.34" y="-45.72" size="1.778" layer="96" rot="R90"/>
</instance>
<instance part="SL3" gate="G$1" x="-29.21" y="-39.37" smashed="yes">
<attribute name="VALUE" x="-31.75" y="-44.45" size="1.778" layer="96"/>
<attribute name="NAME" x="-31.75" y="-33.528" size="1.778" layer="95"/>
</instance>
<instance part="K4" gate="1" x="11.43" y="-36.83" smashed="yes" rot="R270">
<attribute name="VALUE" x="14.351" y="-38.1" size="1.778" layer="96" rot="R270"/>
<attribute name="PART" x="16.51" y="-38.1" size="1.778" layer="95" rot="R270"/>
</instance>
<instance part="K4" gate="2" x="8.89" y="-24.13" smashed="yes" rot="MR90">
<attribute name="PART" x="8.89" y="-21.59" size="1.778" layer="95" rot="MR90"/>
</instance>
<instance part="PE12" gate="M" x="39.37" y="-85.09" smashed="yes">
<attribute name="VALUE" x="36.83" y="-87.63" size="1.778" layer="96" rot="R90"/>
</instance>
<instance part="K6" gate="1" x="33.02" y="-82.55" smashed="yes" rot="R90">
<attribute name="VALUE" x="30.099" y="-81.28" size="1.778" layer="96" rot="R90"/>
<attribute name="PART" x="27.94" y="-81.28" size="1.778" layer="95" rot="R90"/>
</instance>
<instance part="K6" gate="2" x="35.56" y="-66.04" smashed="yes" rot="R90">
<attribute name="PART" x="35.56" y="-63.5" size="1.778" layer="95" rot="R90"/>
</instance>
<instance part="K9" gate="1" x="34.29" y="-49.53" smashed="yes" rot="R90">
<attribute name="VALUE" x="31.369" y="-48.26" size="1.778" layer="96" rot="R90"/>
<attribute name="PART" x="29.21" y="-48.26" size="1.778" layer="95" rot="R90"/>
</instance>
<instance part="K9" gate="2" x="34.29" y="-36.83" smashed="yes" rot="R90">
<attribute name="PART" x="34.29" y="-34.29" size="1.778" layer="95" rot="R90"/>
</instance>
</instances>
<busses>
</busses>
<nets>
<net name="+12V" class="0">
<segment>
<pinref part="SUPPLY1" gate="+12V" pin="+12V"/>
<pinref part="K2" gate="2" pin="P"/>
<wire x1="-6.35" y1="62.23" x2="6.35" y2="62.23" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="D1" gate="1" pin="C"/>
<wire x1="80.01" y1="72.39" x2="85.09" y2="72.39" width="0.1524" layer="91"/>
<wire x1="85.09" y1="72.39" x2="85.09" y2="60.96" width="0.1524" layer="91"/>
<pinref part="D2" gate="1" pin="C"/>
<wire x1="85.09" y1="60.96" x2="80.01" y2="60.96" width="0.1524" layer="91"/>
<wire x1="85.09" y1="60.96" x2="99.06" y2="60.96" width="0.1524" layer="91"/>
<junction x="85.09" y="60.96"/>
<pinref part="P+1" gate="1" pin="+12V"/>
</segment>
<segment>
<pinref part="SUPPLY2" gate="+12V" pin="+12V"/>
<wire x1="-2.54" y1="-116.84" x2="10.16" y2="-116.84" width="0.1524" layer="91"/>
<pinref part="K7" gate="2" pin="P"/>
</segment>
<segment>
<pinref part="D3" gate="1" pin="C"/>
<wire x1="83.82" y1="-106.68" x2="88.9" y2="-106.68" width="0.1524" layer="91"/>
<wire x1="88.9" y1="-106.68" x2="88.9" y2="-118.11" width="0.1524" layer="91"/>
<pinref part="D4" gate="1" pin="C"/>
<wire x1="88.9" y1="-118.11" x2="83.82" y2="-118.11" width="0.1524" layer="91"/>
<wire x1="88.9" y1="-118.11" x2="102.87" y2="-118.11" width="0.1524" layer="91"/>
<junction x="88.9" y="-118.11"/>
<pinref part="P+5" gate="1" pin="+12V"/>
</segment>
<segment>
<pinref part="SUPPLY3" gate="+12V" pin="+12V"/>
<wire x1="-6.35" y1="-24.13" x2="6.35" y2="-24.13" width="0.1524" layer="91"/>
<pinref part="K4" gate="2" pin="P"/>
</segment>
<segment>
<pinref part="D5" gate="1" pin="C"/>
<wire x1="80.01" y1="-13.97" x2="85.09" y2="-13.97" width="0.1524" layer="91"/>
<wire x1="85.09" y1="-13.97" x2="85.09" y2="-25.4" width="0.1524" layer="91"/>
<pinref part="D6" gate="1" pin="C"/>
<wire x1="85.09" y1="-25.4" x2="80.01" y2="-25.4" width="0.1524" layer="91"/>
<wire x1="85.09" y1="-25.4" x2="99.06" y2="-25.4" width="0.1524" layer="91"/>
<junction x="85.09" y="-25.4"/>
<pinref part="P+9" gate="1" pin="+12V"/>
</segment>
</net>
<net name="N$2" class="0">
<segment>
<pinref part="K2" gate="2" pin="S"/>
<pinref part="D1" gate="1" pin="A"/>
<wire x1="13.97" y1="67.31" x2="13.97" y2="72.39" width="0.1524" layer="91"/>
<wire x1="13.97" y1="72.39" x2="72.39" y2="72.39" width="0.1524" layer="91"/>
<pinref part="12/230V" gate="1" pin="P2"/>
<wire x1="72.39" y1="72.39" x2="74.93" y2="72.39" width="0.1524" layer="91"/>
<wire x1="69.85" y1="17.78" x2="72.39" y2="17.78" width="0.1524" layer="91"/>
<wire x1="72.39" y1="17.78" x2="72.39" y2="72.39" width="0.1524" layer="91"/>
<junction x="72.39" y="72.39"/>
</segment>
</net>
<net name="GND" class="0">
<segment>
<pinref part="230/12V" gate="1" pin="S3"/>
<pinref part="GND1" gate="1" pin="GND"/>
<wire x1="68.58" y1="53.34" x2="100.33" y2="53.34" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="230/12V1" gate="1" pin="S3"/>
<pinref part="GND4" gate="1" pin="GND"/>
<wire x1="72.39" y1="-125.73" x2="104.14" y2="-125.73" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="GND5" gate="1" pin="GND"/>
<wire x1="74.93" y1="-175.26" x2="74.93" y2="-173.99" width="0.1524" layer="91"/>
<pinref part="12/230V1" gate="1" pin="P1"/>
<wire x1="74.93" y1="-173.99" x2="73.66" y2="-173.99" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="GND2" gate="1" pin="GND"/>
<wire x1="71.12" y1="3.81" x2="71.12" y2="5.08" width="0.1524" layer="91"/>
<pinref part="12/230V" gate="1" pin="P1"/>
<wire x1="71.12" y1="5.08" x2="69.85" y2="5.08" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="230/12V2" gate="1" pin="S3"/>
<pinref part="GND7" gate="1" pin="GND"/>
<wire x1="68.58" y1="-33.02" x2="100.33" y2="-33.02" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="GND8" gate="1" pin="GND"/>
<wire x1="71.12" y1="-82.55" x2="71.12" y2="-81.28" width="0.1524" layer="91"/>
<pinref part="12/230V2" gate="1" pin="P1"/>
<wire x1="71.12" y1="-81.28" x2="69.85" y2="-81.28" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$3" class="0">
<segment>
<pinref part="K1" gate="1" pin="1"/>
<wire x1="27.94" y1="3.81" x2="20.32" y2="3.81" width="0.1524" layer="91"/>
<wire x1="20.32" y1="3.81" x2="20.32" y2="44.45" width="0.1524" layer="91"/>
<pinref part="K3" gate="2" pin="S"/>
<wire x1="20.32" y1="44.45" x2="29.21" y2="44.45" width="0.1524" layer="91"/>
</segment>
</net>
<net name="VCC" class="0">
<segment>
<pinref part="P+2" gate="VCC" pin="VCC"/>
<pinref part="K3" gate="2" pin="P"/>
<wire x1="39.37" y1="49.53" x2="36.83" y2="49.53" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="P+4" gate="VCC" pin="VCC"/>
<pinref part="K1" gate="2" pin="P"/>
<wire x1="38.1" y1="31.75" x2="100.33" y2="31.75" width="0.1524" layer="91"/>
<wire x1="38.1" y1="20.32" x2="38.1" y2="31.75" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="K1" gate="2" pin="O"/>
<pinref part="P+3" gate="VCC" pin="VCC"/>
</segment>
<segment>
<pinref part="P+6" gate="VCC" pin="VCC"/>
<wire x1="43.18" y1="-129.54" x2="40.64" y2="-129.54" width="0.1524" layer="91"/>
<pinref part="K8" gate="2" pin="P"/>
</segment>
<segment>
<pinref part="P+8" gate="VCC" pin="VCC"/>
<wire x1="41.91" y1="-147.32" x2="104.14" y2="-147.32" width="0.1524" layer="91"/>
<wire x1="41.91" y1="-158.75" x2="41.91" y2="-147.32" width="0.1524" layer="91"/>
<pinref part="K5" gate="2" pin="P"/>
</segment>
<segment>
<pinref part="P+7" gate="VCC" pin="VCC"/>
<pinref part="K5" gate="2" pin="S"/>
</segment>
<segment>
<pinref part="P+10" gate="VCC" pin="VCC"/>
<wire x1="39.37" y1="-36.83" x2="36.83" y2="-36.83" width="0.1524" layer="91"/>
<pinref part="K9" gate="2" pin="P"/>
</segment>
<segment>
<pinref part="P+12" gate="VCC" pin="VCC"/>
<wire x1="38.1" y1="-54.61" x2="100.33" y2="-54.61" width="0.1524" layer="91"/>
<wire x1="38.1" y1="-66.04" x2="38.1" y2="-54.61" width="0.1524" layer="91"/>
<pinref part="K6" gate="2" pin="P"/>
</segment>
<segment>
<pinref part="P+11" gate="VCC" pin="VCC"/>
<pinref part="K6" gate="2" pin="O"/>
</segment>
</net>
<net name="N$4" class="0">
<segment>
<pinref part="K3" gate="2" pin="O"/>
<pinref part="230/12V" gate="1" pin="P1"/>
<wire x1="55.88" y1="63.5" x2="29.21" y2="63.5" width="0.1524" layer="91"/>
<wire x1="29.21" y1="63.5" x2="29.21" y2="54.61" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$1" class="0">
<segment>
<pinref part="D2" gate="1" pin="A"/>
<pinref part="230/12V" gate="1" pin="S4"/>
<wire x1="74.93" y1="60.96" x2="68.58" y2="60.96" width="0.1524" layer="91"/>
</segment>
</net>
<net name="+3V3" class="0">
<segment>
<pinref part="K2" gate="1" pin="2"/>
<pinref part="+3V1" gate="G$1" pin="+3V3"/>
<wire x1="19.05" y1="49.53" x2="16.51" y2="49.53" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="K3" gate="1" pin="2"/>
<pinref part="+3V2" gate="G$1" pin="+3V3"/>
<wire x1="43.18" y1="36.83" x2="39.37" y2="36.83" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="+3V3" gate="G$1" pin="+3V3"/>
<wire x1="22.86" y1="-129.54" x2="20.32" y2="-129.54" width="0.1524" layer="91"/>
<pinref part="K7" gate="1" pin="1"/>
</segment>
<segment>
<pinref part="+3V4" gate="G$1" pin="+3V3"/>
<wire x1="46.99" y1="-142.24" x2="43.18" y2="-142.24" width="0.1524" layer="91"/>
<pinref part="K8" gate="1" pin="2"/>
</segment>
<segment>
<pinref part="+3V5" gate="G$1" pin="+3V3"/>
<wire x1="19.05" y1="-36.83" x2="16.51" y2="-36.83" width="0.1524" layer="91"/>
<pinref part="K4" gate="1" pin="1"/>
</segment>
<segment>
<pinref part="+3V6" gate="G$1" pin="+3V3"/>
<wire x1="43.18" y1="-49.53" x2="39.37" y2="-49.53" width="0.1524" layer="91"/>
<pinref part="K9" gate="1" pin="2"/>
</segment>
</net>
<net name="N$5" class="0">
<segment>
<pinref part="K2" gate="1" pin="1"/>
<wire x1="6.35" y1="49.53" x2="-21.59" y2="49.53" width="0.1524" layer="91"/>
<pinref part="SL1" gate="G$1" pin="2"/>
</segment>
</net>
<net name="N$6" class="0">
<segment>
<pinref part="K3" gate="1" pin="1"/>
<wire x1="29.21" y1="36.83" x2="-21.59" y2="36.83" width="0.1524" layer="91"/>
<pinref part="SL1" gate="G$1" pin="1"/>
<wire x1="-21.59" y1="46.99" x2="-21.59" y2="36.83" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$7" class="0">
<segment>
<pinref part="K1" gate="2" pin="S"/>
<pinref part="12/230V" gate="1" pin="S3"/>
<wire x1="30.48" y1="15.24" x2="57.15" y2="15.24" width="0.1524" layer="91"/>
</segment>
</net>
<net name="PE" class="0">
<segment>
<pinref part="12/230V" gate="1" pin="S4"/>
<wire x1="57.15" y1="7.62" x2="54.61" y2="7.62" width="0.1524" layer="91"/>
<pinref part="PE1" gate="M" pin="PE"/>
<wire x1="54.61" y1="7.62" x2="54.61" y2="-1.27" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="230/12V" gate="1" pin="P2"/>
<pinref part="PE3" gate="M" pin="PE"/>
<wire x1="55.88" y1="50.8" x2="55.88" y2="45.72" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="12/230V1" gate="1" pin="S4"/>
<wire x1="60.96" y1="-171.45" x2="58.42" y2="-171.45" width="0.1524" layer="91"/>
<pinref part="PE4" gate="M" pin="PE"/>
<wire x1="58.42" y1="-171.45" x2="58.42" y2="-180.34" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="230/12V1" gate="1" pin="P2"/>
<pinref part="PE6" gate="M" pin="PE"/>
<wire x1="59.69" y1="-128.27" x2="59.69" y2="-133.35" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="K1" gate="1" pin="2"/>
<wire x1="38.1" y1="3.81" x2="39.37" y2="3.81" width="0.1524" layer="91"/>
<pinref part="PE7" gate="M" pin="PE"/>
</segment>
<segment>
<pinref part="K5" gate="1" pin="2"/>
<wire x1="41.91" y1="-175.26" x2="43.18" y2="-175.26" width="0.1524" layer="91"/>
<pinref part="PE8" gate="M" pin="PE"/>
</segment>
<segment>
<pinref part="12/230V2" gate="1" pin="S4"/>
<wire x1="57.15" y1="-78.74" x2="54.61" y2="-78.74" width="0.1524" layer="91"/>
<pinref part="PE9" gate="M" pin="PE"/>
<wire x1="54.61" y1="-78.74" x2="54.61" y2="-87.63" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="230/12V2" gate="1" pin="P2"/>
<pinref part="PE11" gate="M" pin="PE"/>
<wire x1="55.88" y1="-35.56" x2="55.88" y2="-40.64" width="0.1524" layer="91"/>
</segment>
<segment>
<wire x1="38.1" y1="-82.55" x2="39.37" y2="-82.55" width="0.1524" layer="91"/>
<pinref part="PE12" gate="M" pin="PE"/>
<pinref part="K6" gate="1" pin="2"/>
</segment>
</net>
<net name="N$8" class="0">
<segment>
<pinref part="D3" gate="1" pin="A"/>
<wire x1="17.78" y1="-111.76" x2="17.78" y2="-106.68" width="0.1524" layer="91"/>
<wire x1="17.78" y1="-106.68" x2="76.2" y2="-106.68" width="0.1524" layer="91"/>
<pinref part="12/230V1" gate="1" pin="P2"/>
<wire x1="76.2" y1="-106.68" x2="78.74" y2="-106.68" width="0.1524" layer="91"/>
<wire x1="73.66" y1="-161.29" x2="76.2" y2="-161.29" width="0.1524" layer="91"/>
<wire x1="76.2" y1="-161.29" x2="76.2" y2="-106.68" width="0.1524" layer="91"/>
<junction x="76.2" y="-106.68"/>
<pinref part="K7" gate="2" pin="O"/>
</segment>
</net>
<net name="N$9" class="0">
<segment>
<wire x1="31.75" y1="-175.26" x2="24.13" y2="-175.26" width="0.1524" layer="91"/>
<wire x1="24.13" y1="-175.26" x2="24.13" y2="-134.62" width="0.1524" layer="91"/>
<wire x1="24.13" y1="-134.62" x2="33.02" y2="-134.62" width="0.1524" layer="91"/>
<pinref part="K8" gate="2" pin="O"/>
<pinref part="K5" gate="1" pin="1"/>
</segment>
</net>
<net name="N$10" class="0">
<segment>
<pinref part="230/12V1" gate="1" pin="P1"/>
<wire x1="59.69" y1="-115.57" x2="33.02" y2="-115.57" width="0.1524" layer="91"/>
<wire x1="33.02" y1="-115.57" x2="33.02" y2="-124.46" width="0.1524" layer="91"/>
<pinref part="K8" gate="2" pin="S"/>
</segment>
</net>
<net name="N$11" class="0">
<segment>
<pinref part="D4" gate="1" pin="A"/>
<pinref part="230/12V1" gate="1" pin="S4"/>
<wire x1="78.74" y1="-118.11" x2="72.39" y2="-118.11" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$12" class="0">
<segment>
<wire x1="10.16" y1="-129.54" x2="-17.78" y2="-129.54" width="0.1524" layer="91"/>
<pinref part="SL2" gate="G$1" pin="2"/>
<pinref part="K7" gate="1" pin="2"/>
</segment>
</net>
<net name="N$13" class="0">
<segment>
<wire x1="33.02" y1="-142.24" x2="-17.78" y2="-142.24" width="0.1524" layer="91"/>
<pinref part="SL2" gate="G$1" pin="1"/>
<wire x1="-17.78" y1="-132.08" x2="-17.78" y2="-142.24" width="0.1524" layer="91"/>
<pinref part="K8" gate="1" pin="1"/>
</segment>
</net>
<net name="N$14" class="0">
<segment>
<pinref part="12/230V1" gate="1" pin="S3"/>
<wire x1="34.29" y1="-163.83" x2="60.96" y2="-163.83" width="0.1524" layer="91"/>
<pinref part="K5" gate="2" pin="O"/>
</segment>
</net>
<net name="N$15" class="0">
<segment>
<pinref part="D5" gate="1" pin="A"/>
<wire x1="13.97" y1="-19.05" x2="13.97" y2="-13.97" width="0.1524" layer="91"/>
<wire x1="13.97" y1="-13.97" x2="72.39" y2="-13.97" width="0.1524" layer="91"/>
<pinref part="12/230V2" gate="1" pin="P2"/>
<wire x1="72.39" y1="-13.97" x2="74.93" y2="-13.97" width="0.1524" layer="91"/>
<wire x1="69.85" y1="-68.58" x2="72.39" y2="-68.58" width="0.1524" layer="91"/>
<wire x1="72.39" y1="-68.58" x2="72.39" y2="-13.97" width="0.1524" layer="91"/>
<junction x="72.39" y="-13.97"/>
<pinref part="K4" gate="2" pin="O"/>
</segment>
</net>
<net name="N$16" class="0">
<segment>
<wire x1="27.94" y1="-82.55" x2="20.32" y2="-82.55" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-82.55" x2="20.32" y2="-41.91" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-41.91" x2="29.21" y2="-41.91" width="0.1524" layer="91"/>
<pinref part="K6" gate="1" pin="1"/>
<pinref part="K9" gate="2" pin="S"/>
</segment>
</net>
<net name="N$17" class="0">
<segment>
<pinref part="230/12V2" gate="1" pin="P1"/>
<wire x1="55.88" y1="-22.86" x2="29.21" y2="-22.86" width="0.1524" layer="91"/>
<pinref part="K9" gate="2" pin="O"/>
<wire x1="29.21" y1="-22.86" x2="29.21" y2="-31.75" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$18" class="0">
<segment>
<pinref part="D6" gate="1" pin="A"/>
<pinref part="230/12V2" gate="1" pin="S4"/>
<wire x1="74.93" y1="-25.4" x2="68.58" y2="-25.4" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$19" class="0">
<segment>
<wire x1="6.35" y1="-36.83" x2="-21.59" y2="-36.83" width="0.1524" layer="91"/>
<pinref part="SL3" gate="G$1" pin="2"/>
<pinref part="K4" gate="1" pin="2"/>
</segment>
</net>
<net name="N$20" class="0">
<segment>
<wire x1="29.21" y1="-49.53" x2="-21.59" y2="-49.53" width="0.1524" layer="91"/>
<pinref part="SL3" gate="G$1" pin="1"/>
<wire x1="-21.59" y1="-39.37" x2="-21.59" y2="-49.53" width="0.1524" layer="91"/>
<pinref part="K9" gate="1" pin="1"/>
</segment>
</net>
<net name="N$21" class="0">
<segment>
<pinref part="12/230V2" gate="1" pin="S3"/>
<wire x1="30.48" y1="-71.12" x2="57.15" y2="-71.12" width="0.1524" layer="91"/>
<pinref part="K6" gate="2" pin="S"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
<compatibility>
<note version="8.2" severity="warning">
Since Version 8.2, EAGLE supports online libraries. The ids
of those online libraries will not be understood (or retained)
with this version.
</note>
<note version="8.3" severity="warning">
Since Version 8.3, EAGLE supports URNs for individual library
assets (packages, symbols, and devices). The URNs of those assets
will not be understood (or retained) with this version.
</note>
<note version="8.3" severity="warning">
Since Version 8.3, EAGLE supports the association of 3D packages
with devices in libraries, schematics, and board files. Those 3D
packages will not be understood (or retained) with this version.
</note>
</compatibility>
</eagle>
