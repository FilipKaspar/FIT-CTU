<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="3.0">
    <xsl:variable name="players" select="document('data-1.xml')"/>
    <xsl:variable name="stores" select="document('data-2.xml')"/>
    <xsl:variable name="games" select="document('data-3.xml')"/>
    <xsl:variable name="developers" select="document('data-4.xml')"/>

    <xsl:output method="html" encoding="UTF-8" indent="yes"/>
    <xsl:template match="/">
        <html>
            <head>
                <title>List of Games, Players, Developers and Stores</title>
            </head>
            <body>
                <h1>List of Games, Players, Developers and Stores</h1>
                <xsl:call-template name="tplayers"/>
                <xsl:call-template name="tstores"/>
                <xsl:call-template name="tgames"/>
                <xsl:call-template name="tdevelopers"/>
            </body>
        </html>
    </xsl:template>

    <xsl:template name="tplayers">
        <h2>PLAYERS</h2>
        <xsl:for-each select="$players/Players/Player">
            <p style="display:inline"><b>Iri: </b></p>
            <xsl:value-of select="iri"/>
            <p style="display:inline">&#160;&#160;</p>
            <p style="display:inline"><b>PlayerName: </b></p>
            <xsl:value-of select="playerName"/>
            <p style="display:inline">&#160;&#160;</p>

            <p style="display:inline"><b>Nickname: </b></p>
            <xsl:value-of select="nickname"/>

            <p></p>
        </xsl:for-each>
    </xsl:template>

    <xsl:template name="tstores">
        <h2>STORES</h2>
        <xsl:for-each select="$stores/Stores/Store">
            <p style="display:inline"><b>Iri: </b></p>
            <xsl:value-of select="iri"/>
            <p style="display:inline">&#160;&#160;</p>
            <p style="display:inline"><b>StoreName: </b></p>
            <xsl:value-of select="storeName"/>
            <p style="display:inline">&#160;&#160;</p>

            <p style="display:inline"><b>Reputation: </b></p>
            <xsl:value-of select="reputation"/>

            <p></p>
        </xsl:for-each>
    </xsl:template>

    <xsl:template name="tgames">
        <h2>GAMES</h2>
        <xsl:for-each select="$games/Games/Game">
            <p style="display:inline"><b>Iri: </b></p>
            <xsl:value-of select="iri"/>
            <p style="display:inline">&#160;&#160;</p>
            <p style="display:inline"><b>GameName: </b></p>
            <xsl:value-of select="gameName"/>
            <p style="display:inline">&#160;&#160;</p>

            <p style="display:inline"><b>Revenue: </b></p>
            <xsl:value-of select="revenue"/>

            <p></p>
        </xsl:for-each>
    </xsl:template>

    <xsl:template name="tdevelopers">
        <h2>DEVELOPERS</h2>
        <xsl:for-each select="$developers/Developers/Developer">
            <p style="display:inline"><b>Iri: </b></p>
            <xsl:value-of select="iri"/>
            <p style="display:inline">&#160;&#160;</p>
            <p style="display:inline"><b>DeveloperName: </b></p>
            <xsl:value-of select="developerName"/>
            <p style="display:inline">&#160;&#160;</p>

            <p style="display:inline"><b>Years_of_Experience: </b></p>
            <xsl:value-of select="years_of_experience"/>

            <p></p>
        </xsl:for-each>
    </xsl:template>

</xsl:stylesheet>