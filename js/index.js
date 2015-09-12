var app = angular.module('css_stats', ["chart.js"]).service('dataService', ['$http', function($http) {
    this.getCssFiles = function() {
        return $http.get("ajax/css.php?op=get");
    };
}]).controller('css_file_upload', function($scope, $http, dataService) {
    $scope.charts = {};
    $scope.file_names = {};
    $scope.css_files = [];
    $scope.files_selected = [];
    $scope.css_styles = [];
    $scope.styles_selected = [];
    $scope.pie_labels = [];
    $scope.pie_data = [];
    var init = function() {
            dataService.getCssFiles().success(function(data, status, headers, config) {
                console.log("All records from Database:", data);
                if (data.success) {
                    var css_files = [];
                    var charts = {};
                    var file_names = {};
                    for (var i = 0; i < data.records.length; i++) {
                        var item = data.records[i];
                        var id = item._id.$id;
                        css_files.push({
                            "id": id,
                            "filename": item.filename
                        });
                        charts[id] = item.chart;
                        file_names[id] = item.filename;
                    }
                    $scope.css_files = css_files;
                    $scope.charts = charts;
                    $scope.file_names = file_names;
                } else {
                    console.log("Error:", data.error);
                }
                console.log("All charts:", $scope.charts, $scope.file_names);
            }).error(function(data, status, headers, config) {
                console.log("Error: Ajax get call ", status);
            });
        };
    init();
    var updateSelectedFile = function(action, id) {
            if (action === 'add' && $scope.files_selected.indexOf(id) === -1) {
                $scope.files_selected.push(id);
            }
            if (action === 'remove' && $scope.files_selected.indexOf(id) !== -1) {
                $scope.files_selected.splice($scope.files_selected.indexOf(id), 1);
            }
            updateAvailableStyles();
        };
    var updateSelectedStyle = function(action, style) {
            if (action === 'add' && $scope.styles_selected.indexOf(style) === -1) {
                $scope.styles_selected.push(style);
            }
            if (action === 'remove' && $scope.styles_selected.indexOf(style) !== -1) {
                $scope.styles_selected.splice($scope.styles_selected.indexOf(style), 1);
            }
            updateChart();
        };
    var updateAvailableStyles = function() {
            var all_params = [];
            for (var i = 0; i < $scope.files_selected.length; i++) {
                var id = $scope.files_selected[i];
                var chart = $scope.charts[id];
                for (var k in chart) {
                    if (all_params.indexOf(k) === -1) {
                        all_params.push(k);
                    }
                }
            }
            $scope.styles_selected = [];
            $scope.css_styles = all_params;
        };
    var updateChart = function() {
            var all_labels = [];
            var all_data = [];
            var selected = $scope.styles_selected;
            for (var i = 0; i < $scope.files_selected.length; i++) {
                var id = $scope.files_selected[i];
                var chart = $scope.charts[id];
                for (var k in chart) {
                    if (selected.indexOf(k) !== -1) {
                        for (var m in chart[k]) {
                            all_labels.push($scope.file_names[id] + "|" + k + "|" + m);
                            all_data.push(chart[k][m]);
                        }
                    }
                }
            }
            $scope.pie_labels = all_labels;
            $scope.pie_data = all_data;
            console.log("Pie chart data:", $scope.pie_labels, $scope.pie_data);
        };
    $scope.updateFileSelection = function($event, id) {
        var checkbox = $event.target;
        var action = (checkbox.checked ? 'add' : 'remove');
        updateSelectedFile(action, id);
    };
    $scope.updateStyleSelection = function($event, style) {
        var checkbox = $event.target;
        var action = (checkbox.checked ? 'add' : 'remove');
        updateSelectedStyle(action, style);
    };
    $scope.add_file = function() {
        var file_element = document.getElementById('file').files[0];
        if (file_element.type.match('css.*')) {
            var reader_element = new FileReader();
            reader_element.onloadend = function(e) {
                var data = e.target.result;
                $http.post("ajax/css.php?op=insert", {
                    "filename": file_element.name,
                    "filedata": data
                }).success(function(data, status, headers, config) {
                    if (data.success) {
                        if (data.records.length > 0) {
                            var item = data.records[0];
                            var id = item._id.$id;
                            $scope.css_files.push({
                                "id": id,
                                "filename": item.filename
                            });
                            $scope.charts[id] = item.chart;
                            $scope.file_names[id] = item.filename;
                        } else {
                            console.log("Error: CSS ID not returned", data.records);
                        }
                    } else {
                        console.log("Error:", data.error);
                    }
                }).error(function(data, status, headers, config) {
                    console.log("Error: Ajax insert call ", status);
                });
            };
            reader_element.readAsText(file_element);
        } else {
            console.log("Error: CSS files only");
        }
    };
});