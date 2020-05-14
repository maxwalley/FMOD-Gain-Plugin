studio.plugins.registerPluginDescription("MaxWalley Gain", {

    productName: "Gain Plugin",

    parameters: {
        "gain": { displayName: "Volume" },
    },

    defaultConfigurations: [
        {
            name: "Half Gain",
            parameterValues: {
                "gain": -40,
            },
        },
        {
            name: "Full Gain",
            parameterValues: {
                "gain": 0,
            },
        },
    ],

    deckUi: {
        deckWidgetType: studio.ui.deckWidgetType.Layout,
        layout: studio.ui.layoutType.GridLayout,
        contentsMargin: {
            left: 5, top: 5, right: 5, bottom: 5,
        },
        minimumWidth: 200,
        spacing: 5,
        items: [
            {
                deckWidgetType: studio.ui.deckWidgetType.Pixmap,
                filePath: __dirname + "/600px-Test_grid_4000x4000",
                row: 1,
                column: 1,
                rowSpan: 1,
                columnSpan: 1,
            },

            {
                deckWidgetType: studio.ui.deckWidgetType.Fader,
                binding: "gain",
            },
                ],
    },
});